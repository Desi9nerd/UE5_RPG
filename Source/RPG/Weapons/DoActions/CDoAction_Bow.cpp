#include "Weapons/DoActions/CDoAction_Bow.h"
#include "Global.h"
#include "Characters/CPlayer.h"
#include "Weapons/CEquipment.h"
#include "Weapons/Attachments/CAttachment_Bow.h"
#include "Weapons/AddOns/CArrow.h"
#include "GameFramework/Character.h"
#include "Components/PoseableMeshComponent.h"
#include "Characters/ECharacterTypes.h"
#include "Components/CStateComponent.h"
#include "Components/CMovementComponent.h"
#include "Components/SplineMeshComponent.h"


UCDoAction_Bow::UCDoAction_Bow()
{

}

void UCDoAction_Bow::BeginPlay(ACAttachment* InAttachment, UCEquipment* InEquipment, ACharacter* InOwner, const TArray<FDoActionData>& InDoActionData, const TArray<FHitData>& InHitData)
{
	Super::BeginPlay(InAttachment, InEquipment, InOwner, InDoActionData, InHitData);

	SkeletalMesh = CHelpers::GetComponent<USkeletalMeshComponent>(InAttachment);
	PoseableMesh = CHelpers::GetComponent<UPoseableMeshComponent>(InAttachment);

	//CAnimInstance.h�� Bend������ CAttachment_Bow�� GetBend()�Լ��� ���� ����� �����ͼ� ����Ѵ�.
	ACAttachment_Bow* bow = Cast<ACAttachment_Bow>(InAttachment);
	Bending = bow->GetBend();

	OriginLocation = PoseableMesh->GetBoneLocationByName("bow_string_mid", EBoneSpaces::ComponentSpace);

	bEquipped = InEquipment->GetEquipped();//����� ���� bEquipped������ ���� ���¸� �޾ƿͼ� �־��ش�.

	//Player cast�ϱ�
	PlayerCharacter = Cast<ACPlayer>(OwnerCharacter);

}

void UCDoAction_Bow::DoAction()
{
	CheckFalse(State->IsIdleMode());
	//CheckFalse(State->IsSubActionMode());//Zoom�� �ȵ� ���¿����� �߻簡 �ǵ��� �Ϸ��� �ּ��� ġ�ų� ������ �Ѵ�.

	Super::DoAction();

	DoActionDatas[0].DoAction(OwnerCharacter);
}

void UCDoAction_Bow::Begin_DoAction()//Ȱ �߻�
{
	Super::Begin_DoAction();

	bAttachedString = false;//�߻��ϸ� Ȱ ������ �տ��� ���������� false�� ������ش�.


	*Bending = 0;//Ȱ ���θ� 0���� ������ش�.
	PoseableMesh->SetBoneLocationByName("bow_string_mid", OriginLocation, EBoneSpaces::ComponentSpace);//Ȱ ������ ó�� ��ġ�� ���ư��� ������ش�.

	//ȭ��
	{
		CheckNull(ArrowClass);//ArrowClass �ִ��� Ȯ��.

		ACArrow* arrow = GetAttachedArrow();//Attach�Ǿ� �ִ� ȭ���� �����´�.
		arrow->DetachFromActor(FDetachmentTransformRules(EDetachmentRule::KeepWorld, true));//�߻��ϱ� ���� ȭ���� �����ش�. World �� �������� �����ش�. bInCallModify�� true�� ����.

		//�̺�Ʈ�� �������ش�.
		arrow->OnHit.AddDynamic(this, &UCDoAction_Bow::OnArrowHit);
		arrow->OnEndPlay.AddDynamic(this, &UCDoAction_Bow::OnArrowEndPlay);

		FVector forward = FQuat(OwnerCharacter->GetControlRotation()).GetForwardVector();//������ �����Ѵ�.
		arrow->Shoot(forward);//������ �������� ȭ���� �߻��Ѵ�.
	}
}

void UCDoAction_Bow::End_DoAction()
{
	Super::End_DoAction();

	CreateArrow();//Arrow ����. ȭ���� �߻��� �� ���ο� ȭ���� �����Ѵ�. ���� ������ ȭ���� �տ� �پ��ִ�.
}

void UCDoAction_Bow::OnBeginEquip()
{
	Super::OnBeginEquip();

	OwnerCharacter->GetMesh()->SetCollisionEnabled(ECollisionEnabled::NoCollision);//�浹�� ���ش�.

	CreateArrow();//ó���� ȭ���� �ϳ� �پ��ֵ��� ȭ���� �����Ѵ�.
}

void UCDoAction_Bow::OnUnequip()
{
	Super::OnUnequip();

	*Bending = 0;//Ȱ ���� �־����� Bending�� 0���� �����.
	OwnerCharacter->GetMesh()->SetCollisionEnabled(ECollisionEnabled::QueryOnly);//�浹�� ���ش�.

	PoseableMesh->SetBoneLocationByName("bow_string_mid", OriginLocation, EBoneSpaces::ComponentSpace);//Ȱ ������ ó�� ���·� �����ش�.

	//Ȱ ������ �����ϸ� ȭ����� �����Ѵ�.
	for (int32 i = Arrows.Num() - 1; i >= 0; i--)
	{
		if (!!Arrows[i]->GetAttachParentActor())
			Arrows[i]->Destroy();
	}
}

void UCDoAction_Bow::Tick(float InDeltaTime)
{
	Super::Tick(InDeltaTime);
	
	PoseableMesh->CopyPoseFromSkeletalComponent(SkeletalMesh);

	bool bCheck = true;
	bCheck &= (*bEquipped == true);//���� ���¿��� �Ѵ�.
	bCheck &= (bBeginAction == false);//BeginAction�̸�(Ȱ �߻�) Ȱ ������ ������ �ȵǹǷ� false���� �Ѵ�.
	bCheck &= (bAttachedString == true);//Ȱ ���� �ٴ°� true���� �Ѵ�.

	CheckFalse(bCheck);

	FVector handLocation = OwnerCharacter->GetMesh()->GetSocketLocation("Hand_Bow_Right");
	PoseableMesh->SetBoneLocationByName("bow_string_mid", handLocation, EBoneSpaces::WorldSpace);
}

void UCDoAction_Bow::End_BowString()
{
	*Bending = 100;//Ȱ ������ ���� �ٽ� �־����� ���������.
	bAttachedString = true;//Ȱ ������ �տ� �ٴ´�.
}

void UCDoAction_Bow::CreateArrow()
{
	if (World->bIsTearingDown == true)//World->bIsTearingDown�� World�� ����Ǿ��ٴ� �ǹ�
		return;


	FTransform transform;
	ACArrow* arrow = World->SpawnActorDeferred<ACArrow>(ArrowClass, transform, NULL, NULL, ESpawnActorCollisionHandlingMethod::AlwaysSpawn);
	CheckNull(arrow);

	arrow->AddIgnoreActor(OwnerCharacter);

	FAttachmentTransformRules rule = FAttachmentTransformRules(EAttachmentRule::KeepRelative, true);
	arrow->AttachToComponent(OwnerCharacter->GetMesh(), rule, "Hand_Bow_Right_Arrow");

	Arrows.Add(arrow);
	UGameplayStatics::FinishSpawningActor(arrow, transform);
}

ACArrow* UCDoAction_Bow::GetAttachedArrow()
{
	for (ACArrow* projectile : Arrows)
	{
		if (!!projectile->GetAttachParentActor())
			return projectile;
	}

	return nullptr;
}

void UCDoAction_Bow::OnArrowHit(AActor* InCauser, ACharacter* InOtherCharacter)
{
	CheckFalse(HitDatas.Num() > 0);

	HitDatas[0].SendDamage(OwnerCharacter, InCauser, InOtherCharacter);
}

void UCDoAction_Bow::OnArrowEndPlay(ACArrow* InDestroyer)
{
	Arrows.Remove(InDestroyer);
}

/** ȭ�� ����
 *
 */

void UCDoAction_Bow::GetStartAndEndforTrace()
{
	APlayerCameraManager* camManager = GetWorld()->GetFirstPlayerController()->PlayerCameraManager;
	
	CrosshairWorldLocation = camManager->GetCameraLocation();
	ImpactPoint = camManager->GetCameraRotation().Vector();
}

void UCDoAction_Bow::GetArrowSpawnLocationAndRotation()
{
	TArray<AActor*> ignores;
	ignores.Add(OwnerCharacter);

	//start�� CrosshairWorldLocation, end�� ImpactPoint�� �־��ش�. TraceTypeQuery1�� Visibility
	UKismetSystemLibrary::LineTraceSingle(GetWorld(), CrosshairWorldLocation, ImpactPoint, ETraceTypeQuery::TraceTypeQuery1, false, ignores, EDrawDebugTrace::ForOneFrame, HitResult, true);

	if(HitResult.bBlockingHit)
	{
		ImpactPoint = HitResult.ImpactPoint;
	}

	TargetArrowSpawnLocation = PlayerCharacter->GetMesh()->GetSocketTransform(FName("arrow_socket"), RTS_World).GetLocation();
	TargetArrowSpawnRotation = UKismetMathLibrary::MakeRotFromX((ImpactPoint - TargetArrowSpawnLocation));

	ArrowSpawnLocation = UKismetMathLibrary::VInterpTo(ArrowSpawnLocation, TargetArrowSpawnLocation, World->GetDeltaSeconds(), 30.0f);
	ArrowSpawnRotation = UKismetMathLibrary::RInterpTo(ArrowSpawnRotation, TargetArrowSpawnRotation, World->GetDeltaSeconds(), 30.0f);
	
}

void UCDoAction_Bow::ClearArc()
{
	if (SplineMeshes.IsValidIndex(0))
	{
		while (SplineMeshes.IsValidIndex(0))
		{
			//SplineMeshes[0]->DestroyComponent();
			SplineMeshes.RemoveAt(0);
		}
	}

	SplineMeshes.Empty();
	
	PlayerCharacter->ArrowPathSpline->ClearSplinePoints(true);
}

void UCDoAction_Bow::ProjectilePath()
{
	FVector StartLocation = ArrowSpawnLocation;
	FVector LaunchVelocity = UKismetMathLibrary::GetForwardVector(ArrowSpawnRotation) * ArrowSpeed;//ArrowSpawnRotation.Vector().ForwardVector * ArrowSpeed;
	TArray<AActor*> ActorsToIgnore;
	ActorsToIgnore.Add(OwnerCharacter);

	//FPredictProjectilePathParams PredictProjectilePathParams = UGameplayStatics::Blueprint_PredictProjectilePath_Advanced();

	////UGameplayStatics::PredictProjectilePath()


	//FCollisionResponseParams ResponseParam;
	//TArray<FVector> OutPathPositions;
	//float SimFrequency = 20.0f; // How frequently to sample the path (in Hz)
	//float MaxSimTime = 2.0f; // Maximum time to simulate the path (in seconds)



	//UGameplayStatics::PredictProjectilePath(
	//	this,
	//	OutPathPositions,
	//	StartLocation,
	//	LaunchVelocity,
	//	false,
	//	SimFrequency,
	//	MaxSimTime,
	//	ECC_Visibility,
	//	ActorsToIgnore,
	//	ResponseParam
	//);
}

void UCDoAction_Bow::UpdateArcSpline()
{
}
