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

	//CAnimInstance.h의 Bend변수를 CAttachment_Bow의 GetBend()함수를 거쳐 여기로 가져와서 사용한다.
	ACAttachment_Bow* bow = Cast<ACAttachment_Bow>(InAttachment);
	Bending = bow->GetBend();

	OriginLocation = PoseableMesh->GetBoneLocationByName("bow_string_mid", EBoneSpaces::ComponentSpace);

	bEquipped = InEquipment->GetEquipped();//헤더에 만든 bEquipped변수에 장착 상태를 받아와서 넣어준다.

	//Player cast하기
	PlayerCharacter = Cast<ACPlayer>(OwnerCharacter);

}

void UCDoAction_Bow::DoAction()
{
	CheckFalse(State->IsIdleMode());
	//CheckFalse(State->IsSubActionMode());//Zoom이 안된 상태에서도 발사가 되도록 하려면 주석을 치거나 지워야 한다.

	Super::DoAction();

	DoActionDatas[0].DoAction(OwnerCharacter);
}

void UCDoAction_Bow::Begin_DoAction()//활 발사
{
	Super::Begin_DoAction();

	bAttachedString = false;//발사하면 활 시위가 손에서 떨어지도록 false로 만들어준다.


	*Bending = 0;//활 구부림 0으로 만들어준다.
	PoseableMesh->SetBoneLocationByName("bow_string_mid", OriginLocation, EBoneSpaces::ComponentSpace);//활 시위를 처음 위치로 돌아가게 만들어준다.

	//화살
	{
		CheckNull(ArrowClass);//ArrowClass 있는지 확인.

		ACArrow* arrow = GetAttachedArrow();//Attach되어 있는 화살을 가져온다.
		arrow->DetachFromActor(FDetachmentTransformRules(EDetachmentRule::KeepWorld, true));//발사하기 위해 화살을 떼어준다. World 상 기준으로 떼어준다. bInCallModify를 true로 설정.

		//이벤트를 연결해준다.
		arrow->OnHit.AddDynamic(this, &UCDoAction_Bow::OnArrowHit);
		arrow->OnEndPlay.AddDynamic(this, &UCDoAction_Bow::OnArrowEndPlay);

		FVector forward = FQuat(OwnerCharacter->GetControlRotation()).GetForwardVector();//방향을 설정한다.
		arrow->Shoot(forward);//설정한 방향으로 화살을 발사한다.
	}
}

void UCDoAction_Bow::End_DoAction()
{
	Super::End_DoAction();

	CreateArrow();//Arrow 생성. 화살을 발사한 후 새로운 화살을 생성한다. 새로 생성된 화살은 손에 붙어있다.
}

void UCDoAction_Bow::OnBeginEquip()
{
	Super::OnBeginEquip();

	OwnerCharacter->GetMesh()->SetCollisionEnabled(ECollisionEnabled::NoCollision);//충돌을 꺼준다.

	CreateArrow();//처음에 화살이 하나 붙어있도록 화살을 생성한다.
}

void UCDoAction_Bow::OnUnequip()
{
	Super::OnUnequip();

	*Bending = 0;//활 시위 휘어지는 Bending을 0으로 만든다.
	OwnerCharacter->GetMesh()->SetCollisionEnabled(ECollisionEnabled::QueryOnly);//충돌을 켜준다.

	PoseableMesh->SetBoneLocationByName("bow_string_mid", OriginLocation, EBoneSpaces::ComponentSpace);//활 시위를 처음 상태로 돌려준다.

	//활 장착을 해제하면 화살들을 삭제한다.
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
	bCheck &= (*bEquipped == true);//장착 상태여야 한다.
	bCheck &= (bBeginAction == false);//BeginAction이면(활 발사) 활 시위가 붙으면 안되므로 false여야 한다.
	bCheck &= (bAttachedString == true);//활 시위 붙는게 true여야 한다.

	CheckFalse(bCheck);

	FVector handLocation = OwnerCharacter->GetMesh()->GetSocketLocation("Hand_Bow_Right");
	PoseableMesh->SetBoneLocationByName("bow_string_mid", handLocation, EBoneSpaces::WorldSpace);
}

void UCDoAction_Bow::End_BowString()
{
	*Bending = 100;//활 시위를 땡겨 다시 휘어지게 만들어진다.
	bAttachedString = true;//활 시위가 손에 붙는다.
}

void UCDoAction_Bow::CreateArrow()
{
	if (World->bIsTearingDown == true)//World->bIsTearingDown는 World가 종료되었다는 의미
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

/** 화살 궤적
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

	//start에 CrosshairWorldLocation, end에 ImpactPoint를 넣어준다. TraceTypeQuery1은 Visibility
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
