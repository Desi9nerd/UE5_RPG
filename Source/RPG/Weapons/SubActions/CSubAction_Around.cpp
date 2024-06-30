#include "Weapons/SubActions/CSubAction_Around.h"
#include "Global.h"
#include "GameFramework/Character.h"
#include "Components/CStateComponent.h"
#include "Components/CMovementComponent.h"
#include "Weapons/AddOns/CThornObject.h"

void UCSubAction_Around::Pressed()
{
	Super::Pressed();

	CheckTrue(State->IsSubActionMode());
	State->OnSubActionMode();

	ActionData.DoAction(Owner);
}

void UCSubAction_Around::Begin_SubAction_Implementation()
{
	Super::Begin_SubAction_Implementation();

	FActorSpawnParameters params;
	params.Owner = Owner;
	params.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	FTransform transform;

	FVector ownerLocation = Owner->GetActorLocation();
	ownerLocation += Owner->GetActorRotation().RotateVector(ObjectLocation);

	transform.SetLocation(ownerLocation);

	Owner->GetWorld()->SpawnActor<ACThornObject>(ObjectClass, transform, params);
}

void UCSubAction_Around::End_SubAction_Implementation()
{
	Super::End_SubAction_Implementation();
	
	State->OffSubActionMode();
	State->SetIdleMode();
	Movement->Move();
}