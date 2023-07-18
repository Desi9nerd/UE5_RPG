#include "Weapons/Attachments/CAttachment_Bow.h"
#include "Global.h"
#include "Weapons/AnimInstances/CAnimInstance_Bow.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/PoseableMeshComponent.h"

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
}

void ACAttachment_Bow::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	//ĸó�� Do_Action_Bow������ �ű��.
	//PoseableMesh->CopyPoseFromSkeletalComponent(SkeletalMesh);
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
