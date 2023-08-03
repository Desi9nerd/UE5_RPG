#include "CDoAction.h"
#include "Global.h"
#include "CAttachment.h"
#include "CEquipment.h"
#include "GameFramework/Character.h"
#include "Components/CStateComponent.h"
#include "Components/CMovementComponent.h"

UCDoAction::UCDoAction()
{
}
void UCDoAction::BeginPlay(ACAttachment* InAttachment, UCEquipment* InEquipment, ACharacter* InOwner, const TArray<FDoActionData>& InDoActionDatas, const TArray<FHitData>& InHitDatas)
{
	OwnerCharacter = InOwner;
	World = OwnerCharacter->GetWorld();

	State = CHelpers::GetComponent<UCStateComponent>(OwnerCharacter);
	Movement = CHelpers::GetComponent<UCMovementComponent>(OwnerCharacter);

	DoActionDatas = InDoActionDatas;
	HitDatas = InHitDatas;
}

void UCDoAction::BeginPlay(ACAttachment* InAttachment, UCEquipment* InEquipment, ACharacter* InOwner,
	const TArray<FDoActionData>& InDoActionDatas, const TArray<FHitData>& InHitDatas,
	const TArray<FDoActionData>& InAirborneATKDatas, const TArray<FHitData>& InAirborneATKHitDatas,
	const TArray<FDoActionData>& InDoAirActionDatas, const TArray<FHitData>& InAirHitDatas)
{
	OwnerCharacter = InOwner;
	World = OwnerCharacter->GetWorld();

	State = CHelpers::GetComponent<UCStateComponent>(OwnerCharacter);
	Movement = CHelpers::GetComponent<UCMovementComponent>(OwnerCharacter);

	DoActionDatas = InDoActionDatas;
	HitDatas = InHitDatas;
	DoActionDatas_AirborneInitATK = InAirborneATKDatas;
	HitDatas_AirborneInitATK = InAirborneATKHitDatas;
	DoActionDatas_AirCombo = InDoAirActionDatas;
	HitDatas_AirCombo = InAirHitDatas;

}

void UCDoAction::DoAction()
{
	bInAction = true;

	State->SetActionMode();
}

void UCDoAction::Begin_DoAction()
{
	bBeginAction = true;
}

void UCDoAction::End_DoAction()
{
	bInAction = false;
	bBeginAction = false;

	State->SetIdleMode();

	Movement->Move();
	Movement->DisableFixedCamera();
}

void UCDoAction::DoAction_AirCombo()
{
	bInAction_AirCombo = true;

	State->SetActionMode();
}

void UCDoAction::Begin_DoAction_AirCombo()
{
	bBeginAction_AirCombo = true;
}

void UCDoAction::End_DoAction_AirCombo()
{
	bInAction_AirCombo = false;
	bBeginAction_AirCombo = false;

	State->SetIdleMode();

	Movement->Move();
	Movement->DisableFixedCamera();
}

void UCDoAction::Parrying_Start()
{
}

void UCDoAction::Parrying_End()
{
}
