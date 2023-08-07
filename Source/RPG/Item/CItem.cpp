#include "CItem.h"
#include "Components/SphereComponent.h"
#include "Interfaces/IPickup.h"
#include "NiagaraComponent.h"
#include "NiagaraFunctionLibrary.h"
#include "Kismet/GameplayStatics.h"

ACItem::ACItem()
{
	PrimaryActorTick.bCanEverTick = true;

	ItemMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("ItemMeshComponent"));
	ItemMesh->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);//Collision Respon를 ///Ignore로 설정.
	ItemMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);//Collsion을 NoCollision으로 설정.
	ItemMesh->SetupAttachment(RootComponent);

	//SphereComponent
	Sphere = CreateDefaultSubobject<USphereComponent>(TEXT("Sphere"));
	Sphere->AttachToComponent(ItemMesh, FAttachmentTransformRules::KeepRelativeTransform);
	//Sphere->SetupAttachment(ItemMesh);
	//NiagaraComponent
	ItemEffect = CreateDefaultSubobject<UNiagaraComponent>(TEXT("Embers"));
	ItemEffect->SetupAttachment(ItemMesh); 
}

void ACItem::BeginPlay() 
{
	Super::BeginPlay();

	//Delegate 연결.
	Sphere->OnComponentBeginOverlap.AddDynamic(this, &ACItem::OnSphereOverlap);
	Sphere->OnComponentEndOverlap.AddDynamic(this, &ACItem::OnSphereEndOverlap);
}

float ACItem::TransformedSin()
{
	return Amplitude * FMath::Sin(RunningTime * TimeConstant);
}

float ACItem::TransformedCos()
{
	return Amplitude * FMath::Cos(RunningTime * TimeConstant);
}

//Item 충돌 시작
void ACItem::OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	IIPickup* PickupInterface = Cast<IIPickup>(OtherActor);//OtherActor 캐스팅
	if (PickupInterface)//PickupInterface가 있다면(=OtherActor가 있다면)
	{
		PickupInterface->SetOverlappingItem(this);//PickupInterface와 아이템이 Overlapping된다면 this환//한다.
	}
}

//Item 충돌 끝
void ACItem::OnSphereEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	IIPickup* PickupInterface = Cast<IIPickup>(OtherActor);//OtherActor 캐스팅
	if (PickupInterface)//PickupInterface가 있다면(=OtherActor가 있다면)
	{
		PickupInterface->SetOverlappingItem(nullptr);//nullptr 반환
	}
}

void ACItem::SpawnPickupSystem()
{
	if (PickupEffect)
	{
		UNiagaraFunctionLibrary::SpawnSystemAtLocation(
			this,
			PickupEffect,
			GetActorLocation()
		);
	}
}

void ACItem::SpawnPickupSound()
{
	if (PickupSound)
	{
		UGameplayStatics::SpawnSoundAtLocation(
			this,
			PickupSound,
			GetActorLocation()
		);
	}
}

void ACItem::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	RunningTime += DeltaTime;

	if (ItemState == EItemState::EIS_Hovering)//Item이 떠 있는 상태라면
	{
		AddActorWorldOffset(FVector(1000.f, 0.f, TransformedSin()));
	}
}

