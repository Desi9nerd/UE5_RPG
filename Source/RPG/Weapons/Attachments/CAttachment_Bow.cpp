#include "Weapons/Attachments/CAttachment_Bow.h"
#include "Global.h"
#include "Weapons/AnimInstances/CAnimInstance_Bow.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/PoseableMeshComponent.h"

ACAttachment_Bow::ACAttachment_Bow()
{
	PrimaryActorTick.bCanEverTick = true;//�ǽð� ������ �ǵ��� �־��ش�. �� �־��ָ� Ȱ ������ ���η����°��� ������Ʈ ���� ���� ���� �ִ�.

	CHelpers::CreateComponent<USkeletalMeshComponent>(this, &SkeletalMesh, "SkeletalMesh", Root);
	CHelpers::CreateComponent<UPoseableMeshComponent>(this, &PoseableMesh, "PoseableMesh", Root);


	USkeletalMesh* mesh;
	CHelpers::GetAsset<USkeletalMesh>(&mesh, "SkeletalMesh'/Game/Character/Weapons/ElvenBow/SK_ElvenBow.SK_ElvenBow'");
	SkeletalMesh->SetSkeletalMesh(mesh);
	SkeletalMesh->SetCollisionProfileName("NoCollision");

	TSubclassOf<UCAnimInstance_Bow> animInstacne;
	CHelpers::GetClass<UCAnimInstance_Bow>(&animInstacne, "AnimBlueprint'/Game/Weapons/Bow/ABP_ElvenBow.ABP_ElvenBow_C'");//ABP_ElvenBow ���۷��� �����Ͽ� ����.
	SkeletalMesh->SetAnimInstanceClass(animInstacne);
}

void ACAttachment_Bow::BeginPlay()
{
	Super::BeginPlay();

	AttachTo("Holster_Bow");

	SkeletalMesh->SetVisibility(false);

	PoseableMesh->SetSkeletalMesh(SkeletalMesh->SkeletalMesh);//SkeletalMesh ���� SkeletalMesh ���.
	PoseableMesh->CopyPoseFromSkeletalComponent(SkeletalMesh);//��� ĸó�صд�.
}

void ACAttachment_Bow::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	PoseableMesh->CopyPoseFromSkeletalComponent(SkeletalMesh);
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
