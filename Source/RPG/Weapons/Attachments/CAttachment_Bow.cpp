#include "Weapons/Attachments/CAttachment_Bow.h"
#include "Global.h"
#include "Weapons/AnimInstances/CAnimInstance_Bow.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/PoseableMeshComponent.h"
#include "Characters/CPlayer.h"
#include "Components/SplineMeshComponent.h"

ACAttachment_Bow::ACAttachment_Bow()
{
	PrimaryActorTick.bCanEverTick = true;//�ǽð� ������ �ǵ��� �־��ش�. �� �־��ָ� Ȱ ������ ���η����°��� ������Ʈ ���� ���� ���� �ִ�.
	
	CHelpers::CreateComponent<UPoseableMeshComponent>(this, &PoseableMesh, "PoseableMesh", Root);
	
	USkeletalMesh* mesh;
	CHelpers::GetAsset<USkeletalMesh>(&mesh, "SkeletalMesh'/Game/Character/Weapons/ElvenBow/SK_ElvenBow.SK_ElvenBow'");//ElevenBow ������ �Ҵ��Ѵ�.
	ItemMesh->SetSkeletalMesh(mesh);//�θ� Ŭ������ CItem�� SkeletalMeshComponent ���������� ItemMesh ������ ������ ����. ItemMesh�� mesh(=Elven Bow�� �Ҵ��� USkeletalMesh)�� �Ҵ��Ѵ�.
	ItemMesh->SetCollisionProfileName("NoCollision");
		

	TSubclassOf<UCAnimInstance_Bow> animInstacne;
	CHelpers::GetClass<UCAnimInstance_Bow>(&animInstacne, "AnimBlueprint'/Game/Weapons/Bow/ABP_ElvenBow.ABP_ElvenBow_C'");//ABP_ElvenBow ���۷��� �����Ͽ� ����.
	ItemMesh->SetAnimInstanceClass(animInstacne);

}

void ACAttachment_Bow::BeginPlay()
{
	Super::BeginPlay();

	//AttachTo("Holster_Bow");

	//World�� ��ġ �׽�Ʈ
	//this->SetActorLocation(FVector(100.0f, 200.0f, 50.0f), false, nullptr, ETeleportType::None);//World�� ��ġ

	ItemMesh->SetVisibility(false);

	PoseableMesh->SetSkeletalMesh(ItemMesh->SkeletalMesh);//SkeletalMesh ���� SkeletalMesh ���.
	PoseableMesh->CopyPoseFromSkeletalComponent(ItemMesh);//��� ĸó�صд�.


	//Player cast�ϱ�
	PlayerCharacter = Cast<ACPlayer>(OwnerCharacter);
}

void ACAttachment_Bow::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	GetStartAndEndforTrace();
	GetArrowSpawnLocationAndRotation();
	ClearArc();
	ProjectilePath();
	UpdateArcSpline();
}

void ACAttachment_Bow::OnBeginEquip_Implementation()
{
	Super::OnBeginEquip_Implementation();//CAttachment.h�� �ִ� �Լ�

	AttachTo("Hand_Bow_Left");//Hand_Bow_Left ���Ͽ� ����.


	APlayerController* controller = OwnerCharacter->GetController<APlayerController>();
	if (!!controller)
	{
		OriginViewPitchRange.X = controller->PlayerCameraManager->ViewPitchMin;
		OriginViewPitchRange.Y = controller->PlayerCameraManager->ViewPitchMax;

		controller->PlayerCameraManager->ViewPitchMin = ViewPitchRange.X;
		controller->PlayerCameraManager->ViewPitchMax = ViewPitchRange.Y;
	}
}

void ACAttachment_Bow::OnUnequip_Implementation()
{
	Super::OnUnequip_Implementation();

	AttachTo("Holster_Bow");//Holster_Bow ���Ͽ� ����.


	APlayerController* controller = OwnerCharacter->GetController<APlayerController>();
	if (!!controller)
	{
		controller->PlayerCameraManager->ViewPitchMin = OriginViewPitchRange.X;
		controller->PlayerCameraManager->ViewPitchMax = OriginViewPitchRange.Y;
	}
}

float* ACAttachment_Bow::GetBend()
{
	return Cast<UCAnimInstance_Bow>(ItemMesh->GetAnimInstance())->GetBend();//UCAnimInstance_Bow ���� GetBend() �Լ��� Bend�� ����. UCAnimInstance_Bow ���� Bend���� ����Ѵ�.
}

/** ȭ�� ����
 *
 */

void ACAttachment_Bow::GetStartAndEndforTrace()
{
	APlayerCameraManager* camManager = GetWorld()->GetFirstPlayerController()->PlayerCameraManager;

	CrosshairWorldLocation = camManager->GetCameraLocation();
	ImpactPoint = camManager->GetCameraRotation().Vector();
}

void ACAttachment_Bow::GetArrowSpawnLocationAndRotation()
{
	TArray<AActor*> ignores;
	ignores.Add(OwnerCharacter);

	//start�� CrosshairWorldLocation, end�� ImpactPoint�� �־��ش�. TraceTypeQuery1�� Visibility
	UKismetSystemLibrary::LineTraceSingle(GetWorld(), CrosshairWorldLocation, ImpactPoint, ETraceTypeQuery::TraceTypeQuery1, false, ignores, EDrawDebugTrace::ForOneFrame, HitResult, true, FLinearColor::Red, FLinearColor::Green, 5.0f);

	if (HitResult.bBlockingHit)
	{
		ImpactPoint = HitResult.ImpactPoint;
	}

	TargetArrowSpawnLocation = PlayerCharacter->GetMesh()->GetSocketTransform(FName("Hand_Bow_Right_Arrow"), RTS_World).GetLocation();
	TargetArrowSpawnRotation = UKismetMathLibrary::MakeRotFromX((ImpactPoint - TargetArrowSpawnLocation));

	ArrowSpawnLocation = UKismetMathLibrary::VInterpTo(ArrowSpawnLocation, TargetArrowSpawnLocation, GetWorld()->DeltaTimeSeconds, 30.0f);
	ArrowSpawnRotation = UKismetMathLibrary::RInterpTo(ArrowSpawnRotation, TargetArrowSpawnRotation, GetWorld()->DeltaTimeSeconds, 30.0f);

}

void ACAttachment_Bow::ClearArc()
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

void ACAttachment_Bow::ProjectilePath()
{
	FVector StartLocation = ArrowSpawnLocation;
	FVector LaunchVelocity = UKismetMathLibrary::GetForwardVector(ArrowSpawnRotation) * ArrowSpeed;
	TArray<AActor*> ActorsToIgnore;
	ActorsToIgnore.Add(OwnerCharacter);
	

	FPredictProjectilePathParams PredictParams;
	PredictParams.StartLocation = StartLocation;
	PredictParams.LaunchVelocity = LaunchVelocity;
	PredictParams.bTraceWithCollision = true;
	PredictParams.ProjectileRadius = 5.0f;
	PredictParams.MaxSimTime = 5.0f;
	PredictParams.bTraceWithChannel = true;
	//PredictParams.TraceChannel = ETraceTypeQuery::TraceTypeQuery1;
	PredictParams.ActorsToIgnore = ActorsToIgnore;
	PredictParams.SimFrequency = 20.0f;
	PredictParams.OverrideGravityZ = 0.0f;
	PredictParams.DrawDebugType = EDrawDebugTrace::ForOneFrame;
	PredictParams.DrawDebugTime = 5.0f;
	PredictParams.bTraceComplex = false;

	FPredictProjectilePathResult PredictResult;

	UGameplayStatics::PredictProjectilePath(this, PredictParams, PredictResult);

}

void ACAttachment_Bow::UpdateArcSpline()
{

}
