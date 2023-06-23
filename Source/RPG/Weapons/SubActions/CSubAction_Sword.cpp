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

	bMoving = true;//�̵��� �� �ְ� �������ش�.

	Start = Owner->GetActorLocation();//���� ��ġ = Owner�� ��ġ
	End = Start + Owner->GetActorForwardVector() * Distance;//�� ��ġ = ���� ��ġ + Owner�� �� �������� (������� �⺻�� ������)Distance �Ÿ�

	//Draw Debug
	if (DrawDebug == EDrawDebugTrace::ForDuration)
		DrawDebugDirectionalArrow(Owner->GetWorld(), Start, End, 25, FColor::Green, true, 5, 0, 3);
}

void UCSubAction_Sword::End_SubAction_Implementation()
{
	Super::End_SubAction_Implementation();

	bMoving = false;//������ �������� �̵����� ���ϰ� false�� ������ش�.

	State->SetIdleMode();
	State->OffSubActionMode();

	Movement->Move();
	Movement->DisableFixedCamera();

	if (!!GhostTrail)
		GhostTrail->Destroy();//GhostTrail�� ���ش�.
}

void UCSubAction_Sword::Tick_Implementation(float InDeltaTime)
{
	Super::Tick_Implementation(InDeltaTime);
	CheckFalse(bMoving);

	FVector location = Owner->GetActorLocation();
	float radius = Owner->GetCapsuleComponent()->GetScaledCapsuleRadius();

	//location�� radius ������ ������ End�� ���ٸ�
	//Owner(���⼭�� Player)�� ��ġ�� radius ������ ���� End ��ġ�� �����ߴٸ�
	if (location.Equals(End, radius))
	{
		bMoving = false;//�ϼ� ������ �������� �̵����� ���ϰ� false�� ������ش�.
		Start = End = Owner->GetActorLocation();//���۰� �� ��ġ�� ���� Owner�� ��ġ�� �������ش�. �ʱ�ȭ.

		return;
	}

	FVector direction = (End - Start).GetSafeNormal2D();
	Owner->AddActorWorldOffset(direction * Speed, true);
}

