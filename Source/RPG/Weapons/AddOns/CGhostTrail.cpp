#include "Weapons/AddOns/CGhostTrail.h"
#include "Global.h"
#include "GameFramework/Character.h"
#include "Components/PoseableMeshComponent.h"
#include "Components/CapsuleComponent.h"
#include "Materials/MaterialInstanceConstant.h"
#include "Materials/MaterialInstanceDynamic.h"

ACGhostTrail::ACGhostTrail()
{
	CHelpers::CreateComponent<UPoseableMeshComponent>(this, &Mesh, "Mesh");//PoseableMesh ����
}

void ACGhostTrail::BeginPlay()
{
	Super::BeginPlay();

	UMaterialInstanceConstant* material;
	CHelpers::GetAssetDynamic<UMaterialInstanceConstant>(&material, "MaterialInstanceConstant'/Game/Materials/M_GhostTrail_Inst.M_GhostTrail_Inst'");

	Material = UMaterialInstanceDynamic::Create(material, this);
	Material->SetVectorParameterValue("Color", Color);
	Material->SetScalarParameterValue("Exponent", Exponent);

	Owner = Cast<ACharacter>(GetOwner());


	Mesh->SetVisibility(false);
	Mesh->SetSkeletalMesh(Owner->GetMesh()->SkeletalMesh);//ĸó�� �Ž�
	//SkeletalComponent�� ����ϴ� ������? SkeletalComponent�ȿ� Animation�� ���� �����̴�.
	Mesh->CopyPoseFromSkeletalComponent(Owner->GetMesh());//BeginPlay �� �̸� �ѹ� ĸó ����� �Ѵ�.
	Mesh->SetRelativeScale3D(Scale);

	for (int32 i = 0; i < Owner->GetMesh()->SkeletalMesh->GetMaterials().Num(); i++)
	{
		Mesh->SetMaterial(i, Material);//Material �Ҵ�.
	}


	FTimerDelegate timerDelegate;
	timerDelegate.BindUObject(this, &ACGhostTrail::OnUpdateGhostTrail);

	GetWorld()->GetTimerManager().SetTimer(TimerHandle, timerDelegate, Interval, true, StartDelay);
}

void ACGhostTrail::OnUpdateGhostTrail()
{
	if (false == Mesh->IsVisible())//GhostTrail�� Mesh�� �������־��ٸ�
	{
		Mesh->ToggleVisibility();//���ش�.
	}

	//��ġ����
	float height = Owner->GetCapsuleComponent()->GetScaledCapsuleHalfHeight();
	SetActorLocation(Owner->GetActorLocation() - FVector(ScaleAmount.X, ScaleAmount.Y, height - ScaleAmount.Z));
	SetActorRotation(Owner->GetActorRotation() + FRotator(0.f, -90.f, 0.f));

	Mesh->CopyPoseFromSkeletalComponent(Owner->GetMesh());
}

void ACGhostTrail::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);

	GetWorld()->GetTimerManager().ClearTimer(TimerHandle);//TimerHandle ����.
}
