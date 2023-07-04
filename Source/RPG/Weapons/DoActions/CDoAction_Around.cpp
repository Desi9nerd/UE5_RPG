#include "Weapons/DoActions/CDoAction_Around.h"
#include "Global.h"
#include "GameFramework/Character.h"
#include "Components/CStateComponent.h"

void UCDoAction_Around::DoAction()
{
	CheckFalse(DoActionDatas.Num() > 0);
	CheckFalse(State->IsIdleMode());

	Super::DoAction();

	DoActionDatas[0].DoAction(OwnerCharacter);
}

void UCDoAction_Around::Begin_DoAction()
{
	Super::Begin_DoAction();

	int32 index = UKismetMathLibrary::RandomIntegerInRange(0, RotateClasses.Num() - 1);


	FActorSpawnParameters params;
	params.Owner = OwnerCharacter;
	params.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	OwnerCharacter->GetWorld()->SpawnActor<ACRotate_Object>(RotateClasses[index], params);
}