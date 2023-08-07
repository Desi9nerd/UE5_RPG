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

	TArray<USceneComponent*> children;//최종 부모는 SceneComponent다.
	Root->GetChildrenComponents(true, children);
	for (USceneComponent* child : children)
	{
		UShapeComponent* shape = Cast<UShapeComponent>(child);

		if(!!shape)//shape이 있다면
		{
			//충돌 이벤트 연결
			shape->OnComponentBeginOverlap.AddDynamic(this, &ACAttachment::OnComponentBeginOverlap);
			shape->OnComponentEndOverlap.AddDynamic(this, &ACAttachment::OnComponentEndOverlap);

			Collisions.Add(shape);//Collsions배열에 shape 추가
		}
		OffCollisions();//처음 시작할 때 collision을 꺼준다.
	}

	

	//ACharacter를 먼저 Cast 한 후에 Super::BeginPlay() 호출.
	Super::BeginPlay();	
}

void ACAttachment::OnCollisions()
{
	//Pre (충돌체가 켜지기 전에 작업)
	if (OnAttachmentBeginCollision.IsBound())
		OnAttachmentBeginCollision.Broadcast();//연결되어 있는것 이벤트콜

	for (UShapeComponent* shape : Collisions)//Collisions배열의 for문
		shape->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);//Collision을 QueryAndPhysics 설정.
}

void ACAttachment::OffCollisions()
{
	//Pre (충돌체가 꺼지기 전에 작업)
	if (OnAttachmentEndCollision.IsBound())
		OnAttachmentEndCollision.Broadcast();//연결되어 있는것 이벤트콜

	for (UShapeComponent* shape : Collisions)
		shape->SetCollisionEnabled(ECollisionEnabled::NoCollision);//Collision을 NoCollision 설정.
}

void ACAttachment::OnComponentBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	CheckTrue(OwnerCharacter == OtherActor);//자기 자신
	CheckTrue(OwnerCharacter->GetClass() == OtherActor->GetClass());//GetClass()가 같다는 것은 아군이라는 의미

	//2023.08.07 ImpactPoint를 위해 추가함
	ACAttachment* DamageEvent_CAttachment = NewObject<ACAttachment>();//Create instance of custom damage event
	DamageEvent_CAttachment->HitResult_CAttachment = SweepResult; //Assign the hit result to custom damage event

	if (OnAttachmentBeginOverlap.IsBound())
		OnAttachmentBeginOverlap.Broadcast(OwnerCharacter, this, Cast<ACharacter>(OtherActor));	
}

void ACAttachment::OnComponentEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	CheckTrue(OwnerCharacter == OtherActor);//자기 자신
	CheckTrue(OwnerCharacter->GetClass() == OtherActor->GetClass());//GetClass()가 같다는 것은 아군이라는 의미

	if (OnAttachmentEndOverlap.IsBound())
		OnAttachmentEndOverlap.Broadcast(OwnerCharacter, Cast<ACharacter>(OtherActor));

}

void ACAttachment::AttachTo(FName InSocketName)
{
	AttachToComponent(OwnerCharacter->GetMesh(), FAttachmentTransformRules(EAttachmentRule::KeepRelative, true), InSocketName);

	//무기 줍기 추가
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
////Pickup을 위한 충돌체를 꺼줌.
//void ACAttachment::DisableSphereCollision()
//{
//	if (Sphere)
//	{
//		Sphere->SetCollisionEnabled(ECollisionEnabled::NoCollision);
//	}
//}
//
////Item주위로 나오는 이펙트 꺼줌.
//void ACAttachment::DeactivateEmbers()
//{
//	if (ItemEffect)
//	{
//		ItemEffect->Deactivate();
//	}
//}
