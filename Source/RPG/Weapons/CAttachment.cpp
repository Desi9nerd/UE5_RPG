#include "CAttachment.h"
#include "Global.h"
#include "GameFramework/Character.h"
#include "Components/SceneComponent.h"
#include "Components/ShapeComponent.h"
//////////////////////////////////////
#include "NiagaraComponent.h"
#include "Components/SphereComponent.h"
#include "GameFramework/PawnMovementComponent.h"
//#include "Item/CItem.h"
#include "Interfaces/IHit.h"

ACAttachment::ACAttachment()
{
	CHelpers::CreateComponent(this, &Root, "Root");
}

void ACAttachment::BeginPlay()
{
	OwnerCharacter = Cast<ACharacter>(GetOwner());

	TArray<USceneComponent*> children;//���� �θ�� SceneComponent��.
	Root->GetChildrenComponents(true, children);
	for (USceneComponent* child : children)
	{
		UShapeComponent* shape = Cast<UShapeComponent>(child);

		if(!!shape)//shape�� �ִٸ�
		{
			//�浹 �̺�Ʈ ����
			shape->OnComponentBeginOverlap.AddDynamic(this, &ACAttachment::OnComponentBeginOverlap);
			shape->OnComponentEndOverlap.AddDynamic(this, &ACAttachment::OnComponentEndOverlap);

			Collisions.Add(shape);//Collsions�迭�� shape �߰�
		}
		OffCollisions();//ó�� ������ �� collision�� ���ش�.
	}

	

	//ACharacter�� ���� Cast �� �Ŀ� Super::BeginPlay() ȣ��.
	Super::BeginPlay();	
}

void ACAttachment::OnCollisions()
{
	//Pre (�浹ü�� ������ ���� �۾�)
	if (OnAttachmentBeginCollision.IsBound())
		OnAttachmentBeginCollision.Broadcast();//����Ǿ� �ִ°� �̺�Ʈ��

	for (UShapeComponent* shape : Collisions)//Collisions�迭�� for��
		shape->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);//Collision�� QueryAndPhysics ����.
}

void ACAttachment::OffCollisions()
{
	//Pre (�浹ü�� ������ ���� �۾�)
	if (OnAttachmentEndCollision.IsBound())
		OnAttachmentEndCollision.Broadcast();//����Ǿ� �ִ°� �̺�Ʈ��

	for (UShapeComponent* shape : Collisions)
		shape->SetCollisionEnabled(ECollisionEnabled::NoCollision);//Collision�� NoCollision ����.
}

void ACAttachment::OnComponentBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	CheckTrue(OwnerCharacter == OtherActor);//�ڱ� �ڽ�
	CheckTrue(OwnerCharacter->GetClass() == OtherActor->GetClass());//GetClass()�� ���ٴ� ���� �Ʊ��̶�� �ǹ�

	//2023.08.07 ImpactPoint�� ���� �߰���
	ACAttachment* DamageEvent_CAttachment = NewObject<ACAttachment>();//Create instance of custom damage event
	DamageEvent_CAttachment->HitResult_CAttachment = SweepResult; //Assign the hit result to custom damage event

	if (OnAttachmentBeginOverlap.IsBound())
		OnAttachmentBeginOverlap.Broadcast(OwnerCharacter, this, Cast<ACharacter>(OtherActor));	
}

void ACAttachment::OnComponentEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	CheckTrue(OwnerCharacter == OtherActor);//�ڱ� �ڽ�
	CheckTrue(OwnerCharacter->GetClass() == OtherActor->GetClass());//GetClass()�� ���ٴ� ���� �Ʊ��̶�� �ǹ�

	if (OnAttachmentEndOverlap.IsBound())
		OnAttachmentEndOverlap.Broadcast(OwnerCharacter, Cast<ACharacter>(OtherActor));

}

void ACAttachment::AttachTo(FName InSocketName)
{
	AttachToComponent(OwnerCharacter->GetMesh(), FAttachmentTransformRules(EAttachmentRule::KeepRelative, true), InSocketName);

	//���� �ݱ� �߰�
	//ItemState = EItemState::EIS_Equipped;

	//DisableSphereCollision();
	//PlayEquipSound();
	//DeactivateEmbers();
}

void ACAttachment::AttachToCollision(FName InCollisionName)
{
	for (UShapeComponent* collision : Collisions)
	{
		if (collision->GetName() == InCollisionName.ToString())
		{
			collision->AttachToComponent(OwnerCharacter->GetMesh(), FAttachmentTransformRules(EAttachmentRule::KeepRelative, true), InCollisionName);

			return;
		}
	}
}

///////////////////////////////////////////////////////////////////////////////
//
//void ACAttachment::Equip(USceneComponent* InParent, FName InSocketName, AActor* NewOwner, APawn* NewInstigator)
//{
//	ItemState = EItemState::EIS_Equipped;
//	SetOwner(NewOwner);
//	SetInstigator(NewInstigator);
//
//
//	FAttachmentTransformRules TransformRules(EAttachmentRule::SnapToTarget, true);
//	ItemMesh->AttachToComponent(InParent, TransformRules, InSocketName);
//
//	DisableSphereCollision();
//	PlayEquipSound();
//	DeactivateEmbers();
//}
//
//void ACAttachment::PlayEquipSound()
//{
//	if (EquipSound)
//	{
//		UGameplayStatics::PlaySoundAtLocation(
//			this,
//			EquipSound,
//			GetActorLocation()
//		);
//	}
//}
//
////Pickup�� ���� �浹ü�� ����.
//void ACAttachment::DisableSphereCollision()
//{
//	if (Sphere)
//	{
//		Sphere->SetCollisionEnabled(ECollisionEnabled::NoCollision);
//	}
//}
//
////Item������ ������ ����Ʈ ����.
//void ACAttachment::DeactivateEmbers()
//{
//	if (ItemEffect)
//	{
//		ItemEffect->Deactivate();
//	}
//}
