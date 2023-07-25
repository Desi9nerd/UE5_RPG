#include "Weapons/Attachments/CAttachment_Bow.h"
#include "Global.h"
#include "Weapons/AnimInstances/CAnimInstance_Bow.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/PoseableMeshComponent.h"
#include "Characters/CPlayer.h"
#include "Components/SplineMeshComponent.h"

ACAttachment_Bow::ACAttachment_Bow()
{
	PrimaryActorTick.bCanEverTick = true;//실시간 적용이 되도록 넣어준다. 안 넣어주면 활 시위가 구부러지는것이 업데이트 되지 않을 수도 있다.
	
	CHelpers::CreateComponent<UPoseableMeshComponent>(this, &PoseableMesh, "PoseableMesh", Root);
	
	USkeletalMesh* mesh;
	CHelpers::GetAsset<USkeletalMesh>(&mesh, "SkeletalMesh'/Game/Character/Weapons/ElvenBow/SK_ElvenBow.SK_ElvenBow'");//ElevenBow 에셋을 할당한다.
	ItemMesh->SetSkeletalMesh(mesh);//부모 클래스의 CItem의 SkeletalMeshComponent 데이터형의 ItemMesh 변수를 가져다 쓴다. ItemMesh에 mesh(=Elven Bow를 할당한 USkeletalMesh)를 할당한다.
	ItemMesh->SetCollisionProfileName("NoCollision");
		

	TSubclassOf<UCAnimInstance_Bow> animInstacne;
	CHelpers::GetClass<UCAnimInstance_Bow>(&animInstacne, "AnimBlueprint'/Game/Weapons/Bow/ABP_ElvenBow.ABP_ElvenBow_C'");//ABP_ElvenBow 레퍼런스 복사하여 생성.
	ItemMesh->SetAnimInstanceClass(animInstacne);

}

void ACAttachment_Bow::BeginPlay()
{
	Super::BeginPlay();

	//AttachTo("Holster_Bow");

	//World에 배치 테스트
	//this->SetActorLocation(FVector(100.0f, 200.0f, 50.0f), false, nullptr, ETeleportType::None);//World에 배치

	ItemMesh->SetVisibility(false);

	PoseableMesh->SetSkeletalMesh(ItemMesh->SkeletalMesh);//SkeletalMesh 내의 SkeletalMesh 사용.
	PoseableMesh->CopyPoseFromSkeletalComponent(ItemMesh);//포즈를 캡처해둔다.


	//Player cast하기
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
	Super::OnBeginEquip_Implementation();//CAttachment.h에 있는 함수

	AttachTo("Hand_Bow_Left");//Hand_Bow_Left 소켓에 장착.


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

	AttachTo("Holster_Bow");//Holster_Bow 소켓에 장착.


	APlayerController* controller = OwnerCharacter->GetController<APlayerController>();
	if (!!controller)
	{
		controller->PlayerCameraManager->ViewPitchMin = OriginViewPitchRange.X;
		controller->PlayerCameraManager->ViewPitchMax = OriginViewPitchRange.Y;
	}
}

float* ACAttachment_Bow::GetBend()
{
	return Cast<UCAnimInstance_Bow>(ItemMesh->GetAnimInstance())->GetBend();//UCAnimInstance_Bow 안의 GetBend() 함수로 Bend값 리턴. UCAnimInstance_Bow 안의 Bend값을 사용한다.
}

/** 화살 궤적
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

	//start에 CrosshairWorldLocation, end에 ImpactPoint를 넣어준다. TraceTypeQuery1은 Visibility
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
