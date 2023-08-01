#include "Components/CWeaponComponent.h"
#include "Global.h"
#include "CStateComponent.h"
#include "GameFramework/Character.h"
#include "Weapons/CWeaponAsset.h"
#include "Weapons/CWeaponData.h"
#include "Weapons/CAttachment.h"
#include "Weapons/CEquipment.h"
#include "Weapons/CDoAction.h"
#include "Weapons/CSubAction.h"

UCWeaponComponent::UCWeaponComponent()
{
	//Tick�� ��������ִ� �ڵ�
	PrimaryComponentTick.bCanEverTick = true;
}

void UCWeaponComponent::BeginPlay()
{
	Super::BeginPlay();

	OwnerCharacter = Cast<ACharacter>(GetOwner());
	for (int32 i = 0; i < (int32)EWeaponType::Max; i++)
	{
		if (!!DataAssets[i]) //DataAssets[i]�� �ִٸ�(=���Ⱑ �Ҵ�Ǿ� �ִٸ�)
			DataAssets[i]->BeginPlay(OwnerCharacter, &Datas[i]);//BeginPla y �� OwnerCharacter�� Spawn�����ش�.
	}
}

void UCWeaponComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (!!GetDoAction())//DoAction�� �ִٸ�
		GetDoAction()->Tick(DeltaTime);//DoAction�� Tick�� �� ���ش�.

	if (!!GetSubAction())//SubAction�� �ִٸ�
		GetSubAction()->Tick(DeltaTime);//SubAction�� Tick�� �� ���ش�.

	if (!!GetAirborneInitATK())//AirborneInitATK�� �ִٸ�
		GetAirborneInitATK()->Tick(DeltaTime);//AirborneInitATK�� Tick�� �� ���ش�.
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

	return Datas[(int32)Type]->GetAttachment();//���� �����͸� ����
}

UCEquipment* UCWeaponComponent::GetEquipment()
{
	CheckTrueResult(IsUnarmedMode(), nullptr);
	CheckFalseResult(!!Datas[(int32)Type], nullptr);

	return Datas[(int32)Type]->GetEquipment();//���� �����͸� ����
}

UCDoAction* UCWeaponComponent::GetDoAction()
{
	CheckTrueResult(IsUnarmedMode(), nullptr);
	CheckFalseResult(!!Datas[(int32)Type], nullptr);

	return Datas[(int32)Type]->GetDoAction();//���� �����͸� ����
}

UCSubAction* UCWeaponComponent::GetSubAction()
{
	CheckTrueResult(IsUnarmedMode(), nullptr);
	CheckFalseResult(!!Datas[(int32)Type], nullptr);

	return Datas[(int32)Type]->GetSubAction();//���� �����͸� ����
}

UCDoAction* UCWeaponComponent::GetAirborneInitATK()
{
	CheckTrueResult(IsUnarmedMode(), nullptr);
	CheckFalseResult(!!Datas[(int32)Type], nullptr);

	return Datas[(int32)Type]->GetDoAction_AirborneInitATK();
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
	else if (IsUnarmedMode() == false)
	{
		GetEquipment()->Unequip();
	}

	if (!!Datas[(int32)InType])//���� �����Ͱ� �ִٸ�
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
		OnWeaponTypeChange.Broadcast(prevType, InType);
}

void UCWeaponComponent::DoAction()
{
	if (!!GetDoAction())
		GetDoAction()->DoAction();
}

void UCWeaponComponent::SubAction_Pressed()
{
	if (!!GetSubAction())
		GetSubAction()->Pressed();
}

void UCWeaponComponent::SubAction_Released()
{
	if (!!GetSubAction())
		GetSubAction()->Released();
}

void UCWeaponComponent::AirborneInitATK()
{
	if (!!GetAirborneInitATK())
		GetAirborneInitATK()->AirborneInitATK();
}

void UCWeaponComponent::DoAction_AirCombo()
{
	if (!!GetDoAction())
		GetDoAction()->DoAction_AirCombo();
}
