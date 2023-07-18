#include "Weapons/Attachments/CAttachment_Bow.h"
#include "Global.h"
#include "Weapons/AnimInstances/CAnimInstance_Bow.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/PoseableMeshComponent.h"

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
}

void ACAttachment_Bow::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	//캡처를 Do_Action_Bow쪽으로 옮긴다.
	//PoseableMesh->CopyPoseFromSkeletalComponent(SkeletalMesh);
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
