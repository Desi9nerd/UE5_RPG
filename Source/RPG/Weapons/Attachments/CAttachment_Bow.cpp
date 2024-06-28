#include "Weapons/Attachments/CAttachment_Bow.h"
#include "Global.h"
#include "Weapons/AnimInstances/CAnimInstance_Bow.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/PoseableMeshComponent.h"
#include "Characters/CPlayer.h"
#include "Components/SplineMeshComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Kismet/KismetArrayLibrary.h"
#include "Components/CStateComponent.h"
#include "Weapons/AddOns/CArrow.h"

ACAttachment_Bow::ACAttachment_Bow()
{
	PrimaryActorTick.bCanEverTick = true;//�ǽð� ������ �ǵ��� �־��ش�. �� �־��ָ� Ȱ ������ ���η����°��� ������Ʈ ���� ���� ���� �ִ�.

	CHelpers::CreateComponent<USkeletalMeshComponent>(this, &SkeletalMesh, TEXT("SkeletalMesh"), Root);
	CHelpers::CreateComponent<UPoseableMeshComponent>(this, &PoseableMesh, TEXT("PoseableMesh"), Root);

	USkeletalMesh* mesh;
	CHelpers::GetAsset<USkeletalMesh>(&mesh, "SkeletalMesh'/Game/Character/Weapons/ElvenBow/SK_ElvenBow.SK_ElvenBow'");//ElevenBow ������ �Ҵ��Ѵ�.
	SkeletalMesh->SetSkeletalMesh(mesh);//�θ� Ŭ������ CItem�� SkeletalMeshComponent ���������� ItemMesh ������ ������ ����. ItemMesh�� mesh(=Elven Bow�� �Ҵ��� USkeletalMesh)�� �Ҵ��Ѵ�. XXX //���� ����� �õ��ߴٰ� �ٽ� ����. �ٽ� SkeletalMesh ������� ����.
	SkeletalMesh->SetCollisionProfileName(TEXT("NoCollision"));

	TSubclassOf<UCAnimInstance_Bow> animInstacne;
	CHelpers::GetClass<UCAnimInstance_Bow>(&animInstacne, TEXT("AnimBlueprint'/Game/Weapons/Bow/ABP_ElvenBow.ABP_ElvenBow_C'"));//ABP_ElvenBow ���۷��� �����Ͽ� ����.
	SkeletalMesh->SetAnimInstanceClass(animInstacne);

	CHelpers::CreateComponent<USplineComponent>(this, &ArrowPathSpline, TEXT("ArrowPathSpline"), Root);
	ArcEndSphere = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMeshComponent"));

}

void ACAttachment_Bow::BeginPlay()
{
	Super::BeginPlay();

	AttachTo("Holster_Bow");
	
	SkeletalMesh->SetVisibility(false);

	PoseableMesh->SetSkeletalMesh(SkeletalMesh->SkeletalMesh);//SkeletalMesh ���� SkeletalMesh ���.
	PoseableMesh->CopyPoseFromSkeletalComponent(SkeletalMesh);//��� ĸó�صд�.

	PlayerCharacterCast = Cast<ACPlayer>(OwnerCharacter);	
	state = CHelpers::GetComponent<UCStateComponent>(OwnerCharacter);
}

void ACAttachment_Bow::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	CheckNull(PlayerCharacterCast);
	if (false == state->IsSubActionMode())
	{
		ClearArc();
		ArcEndSphere->SetVisibility(false);
		return;
	}
	
	GetStartAndEndforTrace();
	GetArrowSpawnLocationAndRotation();
	ClearArc();
	FPredictProjectilePathResult InPredictResult = ProjectilePath();
	UpdateArcSpline(InPredictResult);	
}

void ACAttachment_Bow::OnBeginEquip_Implementation()
{
	Super::OnBeginEquip_Implementation();//CAttachment.h�� �ִ� �Լ�

	AttachTo(TEXT("Hand_Bow_Left"));//Hand_Bow_Left ���Ͽ� ����.

	APlayerController* controller = OwnerCharacter->GetController<APlayerController>();
	if (IsValid(controller))
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
	if (IsValid(controller))
	{
		controller->PlayerCameraManager->ViewPitchMin = OriginViewPitchRange.X;
		controller->PlayerCameraManager->ViewPitchMax = OriginViewPitchRange.Y;
	}
}

float* ACAttachment_Bow::GetBend()
{
	return Cast<UCAnimInstance_Bow>(SkeletalMesh->GetAnimInstance())->GetBend();//UCAnimInstance_Bow ���� GetBend() �Լ��� Bend�� ����. UCAnimInstance_Bow ���� Bend���� ����Ѵ�.
}

//---------------------------------------------------------------------------------------
//-- ȭ�� ����

void ACAttachment_Bow::GetStartAndEndforTrace()
{
	APlayerCameraManager* camManager = UGameplayStatics::GetPlayerCameraManager(GetWorld(), 0);

	CrosshairWorldLocation = camManager->GetCameraLocation();
	ImpactPoint = CrosshairWorldLocation + camManager->GetActorForwardVector() * FVector(15000, 15000, 15000);
}

void ACAttachment_Bow::GetArrowSpawnLocationAndRotation()
{
	TArray<AActor*> ignores;
	ignores.Add(OwnerCharacter);
	ignores.Add(this);

	//start�� CrosshairWorldLocation, end�� ImpactPoint�� �־��ش�. TraceTypeQuery1�� Visibility
	UKismetSystemLibrary::LineTraceSingle(GetWorld(), CrosshairWorldLocation, ImpactPoint, ETraceTypeQuery::TraceTypeQuery1, false, ignores, EDrawDebugTrace::None, TraceHitResult, true, FLinearColor::Red, FLinearColor::Green, 5.0f);

	if (TraceHitResult.bBlockingHit)
	{
		ImpactPoint = TraceHitResult.ImpactPoint;
	}

	TargetArrowSpawnLocation = PlayerCharacterCast->GetMesh()->GetSocketTransform(FName("Hand_Bow_Right_Arrow"), RTS_World).GetLocation();
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

	CheckNull(ArrowPathSpline);
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
	PredictParams.TraceChannel = ECollisionChannel::ECC_Pawn;
	PredictParams.ActorsToIgnore = ActorsToIgnore;
	PredictParams.SimFrequency = 20.0f;
	PredictParams.OverrideGravityZ = 0.0f;
	PredictParams.DrawDebugType = EDrawDebugTrace::None; 
	PredictParams.DrawDebugTime = 2.0f;
	PredictParams.bTraceComplex = false;


	FPredictProjectilePathResult PredictResult;

	UGameplayStatics::PredictProjectilePath(this, PredictParams, PredictResult);

	return PredictResult;
}

void ACAttachment_Bow::UpdateArcSpline(FPredictProjectilePathResult InPredictResult)
{
	CheckNull(ArrowPathSpline);
	FPredictProjectilePathResult PredictProjectilePathResult = InPredictResult;

	for (const auto& i : PredictProjectilePathResult.PathData)
	{
		ArrowPathSpline->AddSplinePoint(i.Location, ESplineCoordinateSpace::Local, true);
		ArcEndSphere->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		ArcEndSphere->SetSimulatePhysics(false);
		ArcEndSphere->SetWorldLocation(FinalArcLocation);
		ArcEndSphere->SetVisibility(true);
	}

	ArrowPathSpline->SetSplinePointType((PredictProjectilePathResult.PathData.Num()-1), ESplinePointType::CurveClamped, true);
	FinalArcLocation = PredictProjectilePathResult.PathData[PredictProjectilePathResult.PathData.Num()-1].Location;


	for (int32 i = 0; i < ArrowPathSpline->GetNumberOfSplinePoints() - 1; i++)
	{
		USplineMeshComponent* TempArrowPathSplineMesh = NewObject<USplineMeshComponent>(this, USplineMeshComponent::StaticClass());//ȭ�� ���� �Ž� ����
		
		//ArrowPathSplineMesh->AttachToComponent(OwnerCharacter->GetMesh(), FAttachmentTransformRules::KeepWorldTransform, FName("Hand_Bow_Right_Arrow"));
		TempArrowPathSplineMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		TempArrowPathSplineMesh->bCastDynamicShadow = false;
		TempArrowPathSplineMesh->SetSimulatePhysics(false);
		TempArrowPathSplineMesh->SetStaticMesh(ArrowPathSplineMesh);
		TempArrowPathSplineMesh->SetMaterial(0,ArrowPathSplineMaterial);
		
		SplineMeshes.Add(TempArrowPathSplineMesh);

		FVector pointLocationStart, pointTangentStart, pointLocationEnd, pointTangentEnd;
		ArrowPathSpline->GetLocalLocationAndTangentAtSplinePoint(i, pointLocationStart, pointTangentStart);
		ArrowPathSpline->GetLocalLocationAndTangentAtSplinePoint(i + 1, pointLocationEnd, pointTangentEnd);

		TempArrowPathSplineMesh->SetStartAndEnd(pointLocationStart, pointTangentStart, pointLocationEnd, pointTangentEnd, true);
	}

	RegisterAllComponents();
}

//---------------------------------------------------------------------------------------
