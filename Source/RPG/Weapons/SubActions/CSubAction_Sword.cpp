#include "Weapons/SubActions/CSubAction_Sword.h"
#include "Global.h"
#include "Weapons/CAttachment.h"
#include "Weapons/CDoAction.h"
#include "GameFramework/Character.h"
#include "Components/CapsuleComponent.h"
#include "Components/CStateComponent.h"
#include "Components/CMovementComponent.h"
#include "Weapons/AddOns/CGhostTrail.h"
#include "Components/CTargetComponent.h"

void UCSubAction_Sword::BeginPlay(ACharacter* InOwner, ACAttachment* InAttachment, UCDoAction* InDoAction)
{
	Super::BeginPlay(InOwner, InAttachment, InDoAction);

	Targeting = CHelpers::GetComponent<UCTargetComponent>(Owner);
}

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

	//Begin_SubAction�� ������ �� DoAction�� �浹ü�� ���ְ� SubAction_Sword�� AddDynamic���� �߰����ش�.
	Attachment->OnAttachmentBeginOverlap.Remove(DoAction, TEXT("OnAttachmentBeginOverlap"));
	Attachment->OnAttachmentBeginOverlap.AddDynamic(this, &UCSubAction_Sword::OnAttachmentBeginOverlap);

	bMoving = true;//�̵��� �� �ְ� �������ش�.

	if(false == Target.IsValid())
	{
		CLog::Print(FString("NO Target"));
	}
	else
	{
		Start = Owner->GetActorLocation();//���� ��ġ = Owner�� ��ġ
		End = Start + Owner->GetActorForwardVector() * Distance;//�� ��ġ = ���� ��ġ + Owner�� �� �������� (������� �⺻�� ������)Distance �Ÿ�

		float radius = Owner->GetCapsuleComponent()->GetScaledCapsuleRadius();//capsule�� radius
		float height = Owner->GetCapsuleComponent()->GetScaledCapsuleHalfHeight();//capsule�� halfHeight
		FRotator rotation = Owner->GetActorRotation();

		TArray<AActor*> ignores;//���ܽ�ų �͵��� ���� �迭 ����
		ignores.Add(Owner);//�ڱ��ڽ��� ���ܽ�Ų��.

		TArray<FHitResult> hitResults;
		TArray<TEnumAsByte<EObjectTypeQuery>> objects;
		objects.Add(EObjectTypeQuery::ObjectTypeQuery3);//ObjectTypeQuery3�� Pawn. Pawn�� �����ϱ� ���� objects�� �߰��Ͽ���.

		//Trace�� ������ üũ�ϱ� ���� TraceMulti���
		UKismetSystemLibrary::BoxTraceMultiForObjects(Owner->GetWorld(), Start, End, FVector(0, radius, height), rotation, objects, false, ignores, DrawDebug, hitResults, true);//objects�� ���� Ÿ�Ե�
		for (const FHitResult& hitResult : hitResults)
		{
			ACharacter* character = Cast<ACharacter>(hitResult.GetActor());

			if (IsValid(character))
			{
				character->GetCapsuleComponent()->SetCollisionProfileName(TEXT("Sword_SubAction"));//������Ʈ ���ÿ��� ���� Profile�� Sword_SubAction ���.

				Overlapped.Add(character);
			}
		}

		FHitResult lineHitResult;
		UKismetSystemLibrary::LineTraceSingle(Owner->GetWorld(), Start, End, ETraceTypeQuery::TraceTypeQuery1, false, ignores, DrawDebug, lineHitResult, true);//TraceTypeQuery1�� Visibility
		
		if (lineHitResult.bBlockingHit)//�ϳ��� �浹�Ǿ��ٸ�
		{
			FVector direction = (End - Start).GetSafeNormal2D();
			End = lineHitResult.Location - (direction * radius * 2);//(����) �ε����� �� �����ϴ°� �ƴ϶� capsule * 2 �Ÿ����� �����ߵ��� End ����.

			if (DrawDebug == EDrawDebugTrace::ForDuration)
			{
				DrawDebugSphere(Owner->GetWorld(), End, radius * 2, 20, FColor::Magenta, true, 2);
			}
		}
	}
	
	//Draw Debug
	if (DrawDebug == EDrawDebugTrace::ForDuration)
		DrawDebugDirectionalArrow(Owner->GetWorld(), Start, End, 25, FColor::Green, true, 5, 0, 3);
}

void UCSubAction_Sword::End_SubAction_Implementation()
{
	Super::End_SubAction_Implementation();

	//Begin_SubAction�� ������ SubAction_Sword�� �浹�� ���ְ� DoAction�� �浹ü�� AddDynamic���� �߰��Ѵ�.
	Attachment->OnAttachmentBeginOverlap.Remove(this, TEXT("OnAttachmentBeginOverlap"));
	Attachment->OnAttachmentBeginOverlap.AddDynamic(DoAction, &UCDoAction::OnAttachmentBeginOverlap);

	bMoving = false;//������ �������� �̵����� ���ϰ� false�� ������ش�.

	State->SetIdleMode();
	State->OffSubActionMode();

	Movement->Move();
	Movement->DisableFixedCamera();

	for (ACharacter* character : Overlapped)
	{
		character->GetCapsuleComponent()->SetCollisionProfileName(TEXT("Pawn"));//Sword_SubAction���� �����ߴ� ���������� ���� �������� �̾��� Pawn���� �����ش�.
	}

	if (IsValid(GhostTrail))
	{
		GhostTrail->Destroy();//GhostTrail�� ���ش�.
	}
}

void UCSubAction_Sword::Tick_Implementation(float InDeltaTime)
{
	Super::Tick_Implementation(InDeltaTime);
	CheckFalse(bMoving);

	//FVector location = Owner->GetActorLocation();
	//float radius = Owner->GetCapsuleComponent()->GetScaledCapsuleRadius();
	//
	////location�� radius ������ ������ End�� ���ٸ�
	////Owner(���⼭�� Player)�� ��ġ�� radius ������ ���� End ��ġ�� �����ߴٸ�
	//if (location.Equals(End, radius))
	//{
	//	bMoving = false;//�ϼ� ������ �������� �̵����� ���ϰ� false�� ������ش�.
	//	Start = End = Owner->GetActorLocation();//���۰� �� ��ġ�� ���� Owner�� //��ġ�� �������ش�. �ʱ�ȭ.
	//
	//	return;
	//}
	
	Target = Targeting->GetTarget();

	//Empty�� ������� ������ ���� �ǰ� �Ŀ��� �ǰ��� �߰��� ������� �ʴ´�.
	Overlapped.Empty();
	Hitted.Empty();

	FVector directionToTarget = (Target->GetActorLocation() - Owner->GetActorLocation()).GetSafeNormal2D();
	FVector direction = (End - Start).GetSafeNormal2D();

	if (Target.IsValid()) // Ÿ�� O
	{
		Owner->AddActorWorldOffset(directionToTarget * Speed, true);
	}
	else // Ÿ�� X
	{
		Owner->AddActorWorldOffset(direction * Speed, true);
	}
}

void UCSubAction_Sword::OnAttachmentBeginOverlap(ACharacter* InAttacker, AActor* InAttackCauser, ACharacter* InOther)
{
	CheckNull(InOther);

	for (ACharacter* character : Hitted)
	{
		CheckTrue(character == InOther);
	}

	Hitted.AddUnique(InOther);

	HitData.SendDamage(Owner, InAttackCauser, InOther);//������ ����
}
