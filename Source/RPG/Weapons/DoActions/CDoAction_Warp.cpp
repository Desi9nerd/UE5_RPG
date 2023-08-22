#include "Weapons/DoActions/CDoAction_Warp.h"
#include "Global.h"
#include "GameFramework/Character.h"
#include "GameFramework/PlayerController.h"
#include "Components/CStateComponent.h"
#include "Components/DecalComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/CAIBehaviorComponent.h"
#include "Weapons/CAttachment.h"

UCDoAction_Warp::UCDoAction_Warp()
{

}

void UCDoAction_Warp::BeginPlay(ACAttachment* InAttachment, UCEquipment* InEquipment, ACharacter* InOwner, const TArray<FDoActionData>& InDoActionData, const TArray<FHitData>& InHitData)
{
	Super::BeginPlay(InAttachment, InEquipment, InOwner, InDoActionData, InHitData);

	Decal = CHelpers::GetComponent<UDecalComponent>(InAttachment);
	PlayerController = OwnerCharacter->GetController<APlayerController>();

	Behavior = CHelpers::GetComponent<UCAIBehaviorComponent>(InOwner);
}

void UCDoAction_Warp::Tick(float InDeltaTime)
{
	Super::Tick(InDeltaTime);

	FVector location = FVector::ZeroVector;
	FRotator rotation = FRotator::ZeroRotator;

	//GetCursorLocationAndRotation이 false면 hit가 안 된 것이다.
	if (GetCursorLocationAndRotation(location, rotation) == false)
	{
		Decal->SetVisibility(false);

		return;
	}

	//Warp 실행 중에는 Cursor가 움직이지 않도록 만든다.
	if (bInAction)//Action이 실행중이라면 실행을 할 필요가 없으므로
		return;//리턴


	Decal->SetVisibility(true);
	//Decal이 그려지는 위치와 회전값을 설정해준다.
	Decal->SetWorldLocation(location);
	Decal->SetWorldRotation(rotation);
}

void UCDoAction_Warp::DoAction()
{
	CheckFalse(DoActionDatas.Num() > 0);
	CheckFalse(State->IsIdleMode());

	Super::DoAction();


	FRotator rotation;
	if (GetCursorLocationAndRotation(MoveToLocation, rotation))
	{
		//땅에 뭍히는것을 방지하기 위해 CapsuleHalfHeight만큼 높이를 보정해준다.
		float height = OwnerCharacter->GetCapsuleComponent()->GetScaledCapsuleHalfHeight();
		MoveToLocation = FVector(MoveToLocation.X, MoveToLocation.Y, MoveToLocation.Z + height);

		float yaw = UKismetMathLibrary::FindLookAtRotation(OwnerCharacter->GetActorLocation(), MoveToLocation).Yaw;
		OwnerCharacter->SetActorRotation(FRotator(0, yaw, 0));
	}
	//else
	//	return;

	DoActionDatas[0].DoAction(OwnerCharacter);
}

void UCDoAction_Warp::Begin_DoAction()
{
	Super::Begin_DoAction();

	//Player
	if (!!PlayerController)
	{
		OwnerCharacter->SetActorLocation(MoveToLocation);//DoAction에서 설정한 MoveToLocation 위치로 이동한다.
		MoveToLocation = FVector::ZeroVector;//이동 후에 MoveToLocation 위치를 ZeroVector로 초기화해준다.

		return;//밑의 Enemy AI 기준의 Warp를 실행하지 않기 위해 리턴.
	}

	//Enemy AI
	CheckNull(Behavior);
	OwnerCharacter->SetActorLocation(Behavior->GetAvoidLocation());
}

bool UCDoAction_Warp::GetCursorLocationAndRotation(FVector& OutLocation, FRotator& OutRotation)
{
	CheckNullResult(PlayerController, false);//PlayerController가 없으면 실행하면 안 되니 체크해준다.

	FHitResult hitResult;
	PlayerController->GetHitResultUnderCursorByChannel(ETraceTypeQuery::TraceTypeQuery1, false, hitResult);

	CheckFalseResult(hitResult.bBlockingHit, false);//부딪히는게 없다면 false 리턴.


	OutLocation = hitResult.Location;//hit된 위치
	OutRotation = hitResult.ImpactNormal.Rotation();//hit된 Normal의 회전값

	return true;
}

