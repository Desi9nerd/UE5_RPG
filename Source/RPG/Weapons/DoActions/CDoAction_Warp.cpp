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

	//GetCursorLocationAndRotation�� false�� hit�� �� �� ���̴�.
	if (GetCursorLocationAndRotation(location, rotation) == false)
	{
		Decal->SetVisibility(false);

		return;
	}

	//Warp ���� �߿��� Cursor�� �������� �ʵ��� �����.
	if (bInAction)//Action�� �������̶�� ������ �� �ʿ䰡 �����Ƿ�
		return;//����


	Decal->SetVisibility(true);
	//Decal�� �׷����� ��ġ�� ȸ������ �������ش�.
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
		//���� �����°��� �����ϱ� ���� CapsuleHalfHeight��ŭ ���̸� �������ش�.
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
		OwnerCharacter->SetActorLocation(MoveToLocation);//DoAction���� ������ MoveToLocation ��ġ�� �̵��Ѵ�.
		MoveToLocation = FVector::ZeroVector;//�̵� �Ŀ� MoveToLocation ��ġ�� ZeroVector�� �ʱ�ȭ���ش�.

		return;//���� Enemy AI ������ Warp�� �������� �ʱ� ���� ����.
	}

	//Enemy AI
	CheckNull(Behavior);
	OwnerCharacter->SetActorLocation(Behavior->GetAvoidLocation());
}

bool UCDoAction_Warp::GetCursorLocationAndRotation(FVector& OutLocation, FRotator& OutRotation)
{
	CheckNullResult(PlayerController, false);//PlayerController�� ������ �����ϸ� �� �Ǵ� üũ���ش�.

	FHitResult hitResult;
	PlayerController->GetHitResultUnderCursorByChannel(ETraceTypeQuery::TraceTypeQuery1, false, hitResult);

	CheckFalseResult(hitResult.bBlockingHit, false);//�ε����°� ���ٸ� false ����.


	OutLocation = hitResult.Location;//hit�� ��ġ
	OutRotation = hitResult.ImpactNormal.Rotation();//hit�� Normal�� ȸ����

	return true;
}

