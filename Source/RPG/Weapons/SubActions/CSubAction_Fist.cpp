#include "Weapons/SubActions/CSubAction_Fist.h"
#include "Global.h"
#include "GameFramework/Character.h"
#include "Components/CStateComponent.h"
#include "Components/CMovementComponent.h"
#include "Weapons/CAttachment.h"
#include "Weapons/CDoAction.h"

void UCSubAction_Fist::Pressed()
{
	CheckFalse(State->IsIdleMode());
	CheckTrue(State->IsSubActionMode());

	Super::Pressed();

	State->SetActionMode();
	State->OnSubActionMode();

	ActionData.DoAction(Owner);
}

void UCSubAction_Fist::End_SubAction_Implementation()
{
	Super::End_SubAction_Implementation();

	//원래 상태로 돌려준다.
	State->SetIdleMode();
	State->OffSubActionMode();

	Movement->Move();
	Movement->DisableFixedCamera();
}
