#include "Weapons/Attachments/CAttachment_Bow.h"
#include "Global.h"
#include "Weapons/AnimInstances/CAnimInstance_Bow.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/PoseableMeshComponent.h"
#include "Characters/CPlayer.h"
#include "Components/SplineMeshComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Kismet/KismetArrayLibrary.h"

ACAttachment_Bow::ACAttachment_Bow()
{
	PrimaryActorTick.bCanEverTick = true;//�ǽð� ������ �ǵ��� �־��ش�. �� �־��ָ� Ȱ ������ ���η����°��� ������Ʈ ���� ���� ���� �ִ�.

	CHelpers::CreateComponent<USkeletalMeshComponent>(this, &SkeletalMesh, "SkeletalMesh", Root);
	CHelpers::CreateComponent<UPoseableMeshComponent>(this, &PoseableMesh, "PoseableMesh", Root);

	CHelpers::CreateComponent<USplineComponent>(this, &ArrowPathSpline, "ArrowPathSpline", Root);
	ArcEndSphere = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMeshComponent"));
	ArrowPathSplineMesh = CreateDefaultSubobject<USplineMeshComponent>(TEXT("ArrowPathSplineMesh"));

	USkeletalMesh* mesh;
	CHelpers::GetAsset<USkeletalMesh>(&mesh, "SkeletalMesh'/Game/Character/Weapons/ElvenBow/SK_ElvenBow.SK_ElvenBow'");//ElevenBow ������ �Ҵ��Ѵ�.
	SkeletalMesh->SetSkeletalMesh(mesh);//�θ� Ŭ������ CItem�� SkeletalMeshComponent ���������� ItemMesh ������ ������ ����. ItemMesh�� mesh(=Elven Bow�� �Ҵ��� USkeletalMesh)�� �Ҵ��Ѵ�. XXX //���� ����� �õ��ߴٰ� �ٽ� ����. �ٽ� SkeletalMesh ������� ����.
	SkeletalMesh->SetCollisionProfileName("NoCollision");
		

	TSubclassOf<UCAnimInstance_Bow> animInstacne;
	CHelpers::GetClass<UCAnimInstance_Bow>(&animInstacne, "AnimBlueprint'/Game/Weapons/Bow/ABP_ElvenBow.ABP_ElvenBow_C'");//ABP_ElvenBow ���۷��� �����Ͽ� ����.
	SkeletalMesh->SetAnimInstanceClass(animInstacne);
	
}

void ACAttachment_Bow::BeginPlay()
{
	Super::BeginPlay();

	AttachTo("Holster_Bow");

	//World�� ��ġ �׽�Ʈ
	//this->SetActorLocation(FVector(100.0f, 200.0f, 50.0f), false, nullptr, ETeleportType::None);//World�� ��ġ

	SkeletalMesh->SetVisibility(false);

	PoseableMesh->SetSkeletalMesh(SkeletalMesh->SkeletalMesh);//SkeletalMesh ���� SkeletalMesh ���.
	PoseableMesh->CopyPoseFromSkeletalComponent(SkeletalMesh);//��� ĸó�صд�.


	//Player cast�ϱ�
	PlayerCharacter = Cast<ACPlayer>(OwnerCharacter);
}

void ACAttachment_Bow::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if(!!PlayerCharacter)
	{
		GetStartAndEndforTrace();
		GetArrowSpawnLocationAndRotation();
		ClearArc();
		//FPredictProjectilePathResult InPredictResult = ProjectilePath();
		UpdateArcSpline(ProjectilePath());
	}
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
	return Cast<UCAnimInstance_Bow>(SkeletalMesh->GetAnimInstance())->GetBend();//UCAnimInstance_Bow ���� GetBend() �Լ��� Bend�� ����. UCAnimInstance_Bow ���� Bend���� ����Ѵ�.
}

/** ȭ�� ����
 *
 */

void ACAttachment_Bow::GetStartAndEndforTrace()
{
	APlayerCameraManager* camManager = GetWorld()->GetFirstPlayerController()->PlayerCameraManager;

	CrosshairWorldLocation = camManager->GetCameraLocation();
	ImpactPoint = CrosshairWorldLocation + camManager->GetActorForwardVector() * FVector(15000, 15000, 15000);
}

void ACAttachment_Bow::GetArrowSpawnLocationAndRotation()
{
	TArray<AActor*> ignores;
	ignores.Add(OwnerCharacter);
	ignores.Add(this);

	//start�� CrosshairWorldLocation, end�� ImpactPoint�� �־��ش�. TraceTypeQuery1�� Visibility
	UKismetSystemLibrary::LineTraceSingle(GetWorld(), CrosshairWorldLocation, ImpactPoint, ETraceTypeQuery::TraceTypeQuery1, false, ignores, EDrawDebugTrace::ForOneFrame, TraceHitResult, true, FLinearColor::Red, FLinearColor::Green, 5.0f);

	if (TraceHitResult.bBlockingHit)
	{
		ImpactPoint = TraceHitResult.ImpactPoint;
	}

	TargetArrowSpawnLocation = PlayerCharacter->GetMesh()->GetSocketTransform(FName("Hand_Bow_Right_Arrow"), RTS_World).GetLocation();
	TargetArrowSpawnRotation = UKismetMathLibrary::MakeRotFromX((ImpactPoint - TargetArrowSpawnLocation));

	ArrowSpawnLocation = UKismetMathLibrary::VInterpTo(ArrowSpawnLocation, TargetArrowSpawnLocation, GetWorld()->DeltaTimeSeconds, 30.0f);
	ArrowSpawnRotation = UKismetMathLibrary::RInterpTo(ArrowSpawnRotation, TargetArrowSpawnRotation, GetWorld()->DeltaTimeSeconds, 30.0f);
}

void ACAttachment_Bow::ClearArc()
{	
	for (const auto& i : SplineMeshes)
	{
		i->DestroyComponent();
	}

	SplineMeshes.Empty();
	
	ArrowPathSpline->ClearSplinePoints(true);
}

FPredictProjectilePathResult ACAttachment_Bow::ProjectilePath()
{
	FVector StartLocation = ArrowSpawnLocation;
	FVector LaunchVelocity = UKismetMathLibrary::GetForwardVector(ArrowSpawnRotation) * ArrowSpeed;
	TArray<AActor*> ActorsToIgnore;
	ActorsToIgnore.Add(OwnerCharacter);
	ActorsToIgnore.Add(this);


	FPredictProjectilePathParams PredictParams;
	PredictParams.StartLocation = StartLocation;
	PredictParams.LaunchVelocity = LaunchVelocity;
	PredictParams.bTraceWithCollision = true;
	PredictParams.ProjectileRadius = 5.0f;
	PredictParams.MaxSimTime = 5.0f;
	PredictParams.bTraceWithChannel = true;
	PredictParams.TraceChannel = ECollisionChannel::ECC_Visibility;
	PredictParams.ActorsToIgnore = ActorsToIgnore;
	PredictParams.SimFrequency = 20.0f;
	PredictParams.OverrideGravityZ = 0.0f;
	PredictParams.DrawDebugType = EDrawDebugTrace::ForOneFrame;
	PredictParams.DrawDebugTime = 2.0f;
	PredictParams.bTraceComplex = false;


	FPredictProjectilePathResult PredictResult;

	UGameplayStatics::PredictProjectilePath(this, PredictParams, PredictResult);

	return PredictResult;
}

void ACAttachment_Bow::UpdateArcSpline(FPredictProjectilePathResult InPredictResult)
{
	FPredictProjectilePathResult PredictProjectilePathResult = InPredictResult;

	for (const auto& i : PredictProjectilePathResult.PathData)
	{
		ArrowPathSpline->AddSplinePoint(i.Location, ESplineCoordinateSpace::Local, true);
		ArcEndSphere->SetWorldLocation(FinalArcLocation, false);
	}

	ArrowPathSpline->SetSplinePointType((PredictProjectilePathResult.PathData.Num()-1), ESplinePointType::CurveClamped, true);
	FinalArcLocation = PredictProjectilePathResult.PathData[PredictProjectilePathResult.PathData.Num()-1].Location;


	for (int32 i = 0; i < ArrowPathSpline->GetNumberOfSplinePoints() - 2; i++)
	{
		SplineMeshes.Add(ArrowPathSplineMesh);

		//ArrowPathSplineMesh->bCreatedByConstructionScript = true;
		//ArrowPathSplineMesh->SetMobility(EComponentMobility::Movable);
		//ArrowPathSplineMesh->AttachToComponent(OwnerCharacter->GetMesh(), FAttachmentTransformRules::KeepWorldTransform, FName("Hand_Bow_Right_Arrow"));

		//�� ����. ���� �� �ص� �ȴ�.
		//UMaterialInstanceDynamic* dynamicMat = UMaterialInstanceDynamic::Create(mSplineMeshMaterial, NULL);
		//dynamicMat->SetVectorParameterValue(TEXT("Color"), FLinearColor(mSegments[i].mColor));

		ArrowPathSplineMesh->bCastDynamicShadow = false;
		//ArrowPathSplineMesh->SetStaticMesh(mGridMesh);
		//ArrowPathSplineMesh->SetMaterial(0, dynamicMat);

		//mesh �β�
		ArrowPathSplineMesh->SetStartScale(FVector2D(5, 5));
		ArrowPathSplineMesh->SetEndScale(FVector2D(5, 5));

		FVector pointLocationStart, pointTangentStart, pointLocationEnd, pointTangentEnd;
		ArrowPathSpline->GetLocalLocationAndTangentAtSplinePoint(i, pointLocationStart, pointTangentStart);
		ArrowPathSpline->GetLocalLocationAndTangentAtSplinePoint(i + 1, pointLocationEnd, pointTangentEnd);

		ArrowPathSplineMesh->SetStartAndEnd(pointLocationStart, pointTangentStart, pointLocationEnd, pointTangentEnd, true);
	}

	RegisterAllComponents();
}
