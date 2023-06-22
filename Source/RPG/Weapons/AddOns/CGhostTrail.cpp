#include "Weapons/AddOns/CGhostTrail.h"
#include "Global.h"
#include "GameFramework/Character.h"
#include "Components/PoseableMeshComponent.h"
#include "Components/CapsuleComponent.h"
#include "Materials/MaterialInstanceConstant.h"
#include "Materials/MaterialInstanceDynamic.h"

ACGhostTrail::ACGhostTrail()
{
	CHelpers::CreateComponent<UPoseableMeshComponent>(this, &Mesh, "Mesh");//PoseableMesh 생성
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
	Mesh->SetSkeletalMesh(Owner->GetMesh()->SkeletalMesh);//캡처할 매쉬
	//SkeletalComponent를 사용하는 이유는? SkeletalComponent안에 Animation이 들어가기 때문이다.
	Mesh->CopyPoseFromSkeletalComponent(Owner->GetMesh());//BeginPlay 때 미리 한번 캡처 해줘야 한다.
	Mesh->SetRelativeScale3D(Scale);

	for (int32 i = 0; i < Owner->GetMesh()->SkeletalMesh->Materials.Num(); i++)
		Mesh->SetMaterial(i, Material);//Material 할당.


	FTimerDelegate timerDelegate;
	timerDelegate.BindLambda([=]()
		{
			if (Mesh->IsVisible() == false)//GhostTrail의 Mesh가 숨겨져있었다면
				Mesh->ToggleVisibility();//켜준다.

			//위치보정
			float height = Owner->GetCapsuleComponent()->GetScaledCapsuleHalfHeight();
			SetActorLocation(Owner->GetActorLocation() - FVector(ScaleAmount.X, ScaleAmount.Y, height - ScaleAmount.Z));
			SetActorRotation(Owner->GetActorRotation() + FRotator(0, -90, 0));

			Mesh->CopyPoseFromSkeletalComponent(Owner->GetMesh());
		});

	GetWorld()->GetTimerManager().SetTimer(TimerHandle, timerDelegate, Interval, true, StartDelay);
}

void ACGhostTrail::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);

	GetWorld()->GetTimerManager().ClearTimer(TimerHandle);//TimerHandle 해제.
}
