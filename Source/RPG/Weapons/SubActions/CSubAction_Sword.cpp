#include "Weapons/SubActions/CSubAction_Sword.h"
#include "Global.h"
#include "Weapons/CAttachment.h"
#include "Weapons/CDoAction.h"
#include "GameFramework/Character.h"
#include "Components/CapsuleComponent.h"
#include "Components/CStateComponent.h"
#include "Components/CMovementComponent.h"
#include "Components/CapsuleComponent.h"
#include "Weapons/AddOns/CGhostTrail.h"

void UCSubAction_Sword::Pressed()
{
	CheckFalse(State->IsIdleMode());
	CheckTrue(State->IsSubActionMode());

	Super::Pressed();


	State->SetActionMode();
	State->OnSubActionMode();

	GhostTrail = CHelpers::Play_GhostTrail(GhostTrailClass, Owner);

	ActionData.DoAction(Owner);
}

void UCSubAction_Sword::Begin_SubAction_Implementation()
{
	Super::Begin_SubAction_Implementation();

	bMoving = true;//이동할 수 있게 설정해준다.

	Start = Owner->GetActorLocation();//시작 위치 = Owner의 위치
	End = Start + Owner->GetActorForwardVector() * Distance;//끝 위치 = 시작 위치 + Owner의 앞 방향으로 (헤더에서 기본값 설정한)Distance 거리

	//Draw Debug
	if (DrawDebug == EDrawDebugTrace::ForDuration)
		DrawDebugDirectionalArrow(Owner->GetWorld(), Start, End, 25, FColor::Green, true, 5, 0, 3);
}

void UCSubAction_Sword::End_SubAction_Implementation()
{
	Super::End_SubAction_Implementation();

	bMoving = false;//동작이 끝났으니 이동하지 못하게 false로 만들어준다.

	State->SetIdleMode();
	State->OffSubActionMode();

	Movement->Move();
	Movement->DisableFixedCamera();

	if (!!GhostTrail)
		GhostTrail->Destroy();//GhostTrail를 없앤다.
}

void UCSubAction_Sword::Tick_Implementation(float InDeltaTime)
{
	Super::Tick_Implementation(InDeltaTime);
	CheckFalse(bMoving);

	FVector location = Owner->GetActorLocation();
	float radius = Owner->GetCapsuleComponent()->GetScaledCapsuleRadius();

	//location이 radius 오차값 내에서 End와 같다면
	//Owner(여기서는 Player)의 위치가 radius 오차값 내의 End 위치에 도달했다면
	if (location.Equals(End, radius))
	{
		bMoving = false;//일섬 공격이 끝났으니 이동하지 못하게 false로 만들어준다.
		Start = End = Owner->GetActorLocation();//시작과 끝 위치를 현재 Owner의 위치로 설정해준다. 초기화.

		return;
	}

	FVector direction = (End - Start).GetSafeNormal2D();
	Owner->AddActorWorldOffset(direction * Speed, true);
}

