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

	//Begin_SubAction이 시작할 때 DoAction의 충돌체를 빼주고 SubAction_Sword는 AddDynamic으로 추가해준다.
	Attachment->OnAttachmentBeginOverlap.Remove(DoAction, TEXT("OnAttachmentBeginOverlap"));
	Attachment->OnAttachmentBeginOverlap.AddDynamic(this, &UCSubAction_Sword::OnAttachmentBeginOverlap);

	bMoving = true;//이동할 수 있게 설정해준다.

	if(false == Target.IsValid())
	{
		CLog::Print(FString("NO Target"));
	}
	else
	{
		Start = Owner->GetActorLocation();//시작 위치 = Owner의 위치
		End = Start + Owner->GetActorForwardVector() * Distance;//끝 위치 = 시작 위치 + Owner의 앞 방향으로 (헤더에서 기본값 설정한)Distance 거리

		float radius = Owner->GetCapsuleComponent()->GetScaledCapsuleRadius();//capsule의 radius
		float height = Owner->GetCapsuleComponent()->GetScaledCapsuleHalfHeight();//capsule의 halfHeight
		FRotator rotation = Owner->GetActorRotation();

		TArray<AActor*> ignores;//제외시킬 것들을 담을 배열 변수
		ignores.Add(Owner);//자기자신은 제외시킨다.

		TArray<FHitResult> hitResults;
		TArray<TEnumAsByte<EObjectTypeQuery>> objects;
		objects.Add(EObjectTypeQuery::ObjectTypeQuery3);//ObjectTypeQuery3은 Pawn. Pawn을 추적하기 위해 objects에 추가하였다.

		//Trace를 여러개 체크하기 위해 TraceMulti사용
		UKismetSystemLibrary::BoxTraceMultiForObjects(Owner->GetWorld(), Start, End, FVector(0, radius, height), rotation, objects, false, ignores, DrawDebug, hitResults, true);//objects는 추적 타입들
		for (const FHitResult& hitResult : hitResults)
		{
			ACharacter* character = Cast<ACharacter>(hitResult.GetActor());

			if (IsValid(character))
			{
				character->GetCapsuleComponent()->SetCollisionProfileName(TEXT("Sword_SubAction"));//프로젝트 세팅에서 만든 Profile인 Sword_SubAction 사용.

				Overlapped.Add(character);
			}
		}

		FHitResult lineHitResult;
		UKismetSystemLibrary::LineTraceSingle(Owner->GetWorld(), Start, End, ETraceTypeQuery::TraceTypeQuery1, false, ignores, DrawDebug, lineHitResult, true);//TraceTypeQuery1은 Visibility
		
		if (lineHitResult.bBlockingHit)//하나라도 충돌되었다면
		{
			FVector direction = (End - Start).GetSafeNormal2D();
			End = lineHitResult.Location - (direction * radius * 2);//(벽에) 부딛혔을 때 밀착하는게 아니라 capsule * 2 거리에서 멈춘추도록 End 설정.

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

	//Begin_SubAction이 끝나면 SubAction_Sword의 충돌은 빼주고 DoAction의 충돌체는 AddDynamic으로 추가한다.
	Attachment->OnAttachmentBeginOverlap.Remove(this, TEXT("OnAttachmentBeginOverlap"));
	Attachment->OnAttachmentBeginOverlap.AddDynamic(DoAction, &UCDoAction::OnAttachmentBeginOverlap);

	bMoving = false;//동작이 끝났으니 이동하지 못하게 false로 만들어준다.

	State->SetIdleMode();
	State->OffSubActionMode();

	Movement->Move();
	Movement->DisableFixedCamera();

	for (ACharacter* character : Overlapped)
	{
		character->GetCapsuleComponent()->SetCollisionProfileName(TEXT("Pawn"));//Sword_SubAction으로 변경했던 프로파일을 원래 프로파일 이었던 Pawn으로 돌려준다.
	}

	if (IsValid(GhostTrail))
	{
		GhostTrail->Destroy();//GhostTrail를 없앤다.
	}
}

void UCSubAction_Sword::Tick_Implementation(float InDeltaTime)
{
	Super::Tick_Implementation(InDeltaTime);
	CheckFalse(bMoving);

	//FVector location = Owner->GetActorLocation();
	//float radius = Owner->GetCapsuleComponent()->GetScaledCapsuleRadius();
	//
	////location이 radius 오차값 내에서 End와 같다면
	////Owner(여기서는 Player)의 위치가 radius 오차값 내의 End 위치에 도달했다면
	//if (location.Equals(End, radius))
	//{
	//	bMoving = false;//일섬 공격이 끝났으니 이동하지 못하게 false로 만들어준다.
	//	Start = End = Owner->GetActorLocation();//시작과 끝 위치를 현재 Owner의 //위치로 설정해준다. 초기화.
	//
	//	return;
	//}
	
	Target = Targeting->GetTarget();

	//Empty로 비워주지 않으면 최초 피격 후에는 피격이 추가로 적용되지 않는다.
	Overlapped.Empty();
	Hitted.Empty();

	FVector directionToTarget = (Target->GetActorLocation() - Owner->GetActorLocation()).GetSafeNormal2D();
	FVector direction = (End - Start).GetSafeNormal2D();

	if (Target.IsValid()) // 타겟 O
	{
		Owner->AddActorWorldOffset(directionToTarget * Speed, true);
	}
	else // 타겟 X
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

	HitData.SendDamage(Owner, InAttackCauser, InOther);//데미지 전달
}
