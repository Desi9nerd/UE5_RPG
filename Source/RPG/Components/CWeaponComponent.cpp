#include "Components/CWeaponComponent.h"
#include "Global.h"
#include "CStateComponent.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Weapons/CWeaponAsset.h"
#include "Weapons/CWeaponData.h"
#include "Weapons/CAttachment.h"
#include "Weapons/CEquipment.h"
#include "Weapons/CDoAction.h"
#include "Weapons/CSubAction.h"

UCWeaponComponent::UCWeaponComponent()
{
	//Tick을 실행시켜주는 코드
	PrimaryComponentTick.bCanEverTick = true;
}

void UCWeaponComponent::BeginPlay()
{
	Super::BeginPlay();

	OwnerCharacter = Cast<ACharacter>(GetOwner());
	for (int32 i = 0; i < (int32)EWeaponType::Max; i++)
	{
		if (!!DataAssets[i]) //DataAssets[i]이 있다면(=무기가 할당되어 있다면)
			DataAssets[i]->BeginPlay(OwnerCharacter, &Datas[i]);//BeginPla y 시 OwnerCharacter에 Spawn시켜준다.
	}
}

void UCWeaponComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (IsValid(GetDoAction()))//DoAction이 있다면
	{
		GetDoAction()->Tick(DeltaTime);//DoAction의 Tick을 콜 해준다.
	}

	if (IsValid(GetSubAction()))//SubAction이 있다면
	{
		GetSubAction()->Tick(DeltaTime);//SubAction의 Tick을 콜 해준다.
	}
}

bool UCWeaponComponent::IsIdleMode()
{
	return CHelpers::GetComponent<UCStateComponent>(OwnerCharacter)->IsIdleMode();
}

bool UCWeaponComponent::IsDoActionMode()
{
	return CHelpers::GetComponent<UCDoAction>(OwnerCharacter)->GetInAction();
}

bool UCWeaponComponent::IsDoAction_AirComboMode()
{
	return CHelpers::GetComponent<UCDoAction>(OwnerCharacter)->GetInAction_AirCombo();
}

ACAttachment* UCWeaponComponent::GetAttachment()
{
	CheckTrueResult(IsUnarmedMode(), nullptr);
	CheckFalseResult(!!Datas[(int32)Type], nullptr);

	return Datas[(int32)Type]->GetAttachment();//실제 데이터를 리턴
}

UCEquipment* UCWeaponComponent::GetEquipment()
{
	CheckTrueResult(IsUnarmedMode(), nullptr);
	CheckFalseResult(!!Datas[(int32)Type], nullptr);

	return Datas[(int32)Type]->GetEquipment();//실제 데이터를 리턴
}

UCDoAction* UCWeaponComponent::GetDoAction()
{
	CheckTrueResult(IsUnarmedMode(), nullptr);
	CheckFalseResult(!!Datas[(int32)Type], nullptr);

	return Datas[(int32)Type]->GetDoAction();//실제 데이터를 리턴
}

UCSubAction* UCWeaponComponent::GetSubAction()
{
	CheckTrueResult(IsUnarmedMode(), nullptr);
	CheckFalseResult(!!Datas[(int32)Type], nullptr);

	return Datas[(int32)Type]->GetSubAction();//실제 데이터를 리턴
}

void UCWeaponComponent::SetUnarmedMode()
{
	GetEquipment()->Unequip();

	ChangeType(EWeaponType::Max);
}

void UCWeaponComponent::SetFistMode()
{
	CheckFalse(IsIdleMode());
	SetMode(EWeaponType::Fist);
}

void UCWeaponComponent::SetSwordMode()
{
	CheckFalse(IsIdleMode());
	SetMode(EWeaponType::Sword);
}

void UCWeaponComponent::SetHammerMode()
{
	CheckFalse(IsIdleMode());
	SetMode(EWeaponType::Hammer);
}

void UCWeaponComponent::SetWarpMode()
{
	CheckFalse(IsIdleMode());
	SetMode(EWeaponType::Warp);
}

void UCWeaponComponent::SetAroundMode()
{
	CheckFalse(IsIdleMode());
	SetMode(EWeaponType::Around);
}

void UCWeaponComponent::SetBowMode()
{
	CheckFalse(IsIdleMode());
	SetMode(EWeaponType::Bow);
}

void UCWeaponComponent::SetBladeMode()
{
	CheckFalse(IsIdleMode());
	SetMode(EWeaponType::Blade);
}

void UCWeaponComponent::SetMode(EWeaponType InType)
{
	if (Type == InType)
	{
		SetUnarmedMode();

		return;
	}

	if (false == IsUnarmedMode())
	{
		GetEquipment()->Unequip();
	}

	if (Datas[(int32)InType])//실제 데이터가 있다면
	{
		Datas[(int32)InType]->GetEquipment()->Equip();

		ChangeType(InType);
	}
}

void UCWeaponComponent::ChangeType(EWeaponType InType)
{
	EWeaponType prevType = Type;
	Type = InType;

	if (OnWeaponTypeChange.IsBound())
	{
		OnWeaponTypeChange.Broadcast(prevType, InType);
	}
}

void UCWeaponComponent::DoAction()
{
	if (IsValid(GetDoAction()))
	{
		if(false == OwnerCharacter->GetCharacterMovement()->IsFalling())
		{
			GetDoAction()->DoAction();
		}
		else
		{
			GetDoAction()->DoAction_AirCombo();//공중콤보
		}
	}
}

void UCWeaponComponent::SubAction_Pressed()
{
	if (IsValid(GetSubAction())) GetSubAction()->Pressed();
}

void UCWeaponComponent::SubAction_Released()
{
	if (IsValid(GetSubAction())) GetSubAction()->Released();
}

void UCWeaponComponent::AirborneInitATK()
{
	if (IsValid(GetDoAction())) GetDoAction()->AirborneInitATK();
}

void UCWeaponComponent::DoAction_AirCombo()
{
	if (IsValid(GetDoAction())) GetDoAction()->DoAction_AirCombo();
}

void UCWeaponComponent::Parrying_Pressed()
{
	if (IsValid(GetDoAction())) GetDoAction()->Parrying_Start();
}

void UCWeaponComponent::Parrying_Released()
{
	if (IsValid(GetDoAction())) GetDoAction()->Parrying_End();
}
