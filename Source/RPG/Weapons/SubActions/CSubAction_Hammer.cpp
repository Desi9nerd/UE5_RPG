#include "Weapons/SubActions/CSubAction_Hammer.h"
#include "Global.h"
#include "GameFramework/Character.h"
#include "Components/CStateComponent.h"
#include "Components/CMovementComponent.h"
#include "Weapons/CAttachment.h"
#include "Weapons/CDoAction.h"
#include "Weapons/AddOns/CAura.h"
#include "Weapons/AddOns/CGhostTrail.h"

void UCSubAction_Hammer::Pressed()
{
	CheckFalse(State->IsIdleMode());
	CheckTrue(State->IsSubActionMode());

	Super::Pressed();


	State->SetActionMode();
	State->OnSubActionMode();

	GhostTrail = CHelpers::Play_GhostTrail(GhostTrailClass, Owner);

	ActionData.DoAction(Owner);
}

void UCSubAction_Hammer::Begin_SubAction_Implementation()
{
	Super::Begin_SubAction_Implementation();

	FActorSpawnParameters params;
	params.Owner = Owner;
	params.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;//AlwaysSpawn�� �ִ¾��� ������ Spawn��Ű��� �ǹ�.

	FTransform transform;
	transform.SetLocation(Owner->GetActorLocation());
	transform.AddToTranslation(AuraLoction);//���� ��ġ
	transform.SetRotation(FQuat(Owner->GetActorRotation()));//ĳ������ ��������� ĳ������ ȸ���������� ����.

	Owner->GetWorld()->SpawnActor<ACAura>(AuraClass, transform, params);
}

void UCSubAction_Hammer::End_SubAction_Implementation()
{
	Super::End_SubAction_Implementation();

	State->SetIdleMode();
	State->OffSubActionMode();

	Movement->Move();
	Movement->DisableFixedCamera();

	if (IsValid(GhostTrail))
	{
		GhostTrail->Destroy();
	}
}