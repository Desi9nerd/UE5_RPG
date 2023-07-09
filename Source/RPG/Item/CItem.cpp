#include "CItem.h"
#include "Components/SphereComponent.h"
//#include "Interfaces/PickupInterface.h"
#include "NiagaraComponent.h"
#include "NiagaraFunctionLibrary.h"
#include "Kismet/GameplayStatics.h"

ACItem::ACItem()
{
	PrimaryActorTick.bCanEverTick = true;

	ItemMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("ItemMeshComponent"));
	ItemMesh->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);//Collision Response�� Ignore�� ����.
	ItemMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);//Collsion�� NoCollision���� ����.
	RootComponent = ItemMesh;
	//SphereComponent
	Sphere = CreateDefaultSubobject<USphereComponent>(TEXT("Sphere"));
	Sphere->SetupAttachment(GetRootComponent());
	//NiagaraComponent
	ItemEffect = CreateDefaultSubobject<UNiagaraComponent>(TEXT("Embers"));
	ItemEffect->SetupAttachment(GetRootComponent());
}

void ACItem::BeginPlay()
{
	Super::BeginPlay();

	//Delegate ����.
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

//Item �浹 ����
void ACItem::OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	//IPickupInterface* PickupInterface = Cast<IPickupInterface>(OtherActor);//OtherActor ĳ����
	//if (PickupInterface)//PickupInterface�� �ִٸ�(=OtherActor�� �ִٸ�)
	//{
	//	PickupInterface->SetOverlappingItem(this);//PickupInterface�� �������� Overlapping�ȴٸ� this�� ��ȯ�Ѵ�.
	//}
}

//Item �浹 ��
void ACItem::OnSphereEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	//IPickupInterface* PickupInterface = Cast<IPickupInterface>(OtherActor);//OtherActor ĳ����
	//if (PickupInterface)//PickupInterface�� �ִٸ�(=OtherActor�� �ִٸ�)
	//{
	//	PickupInterface->SetOverlappingItem(nullptr);//nullptr ��ȯ
	//}
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

	if (ItemState == EItemState::EIS_Hovering)//Item�� �� �ִ� ���¶��
	{
		AddActorWorldOffset(FVector(0.f, 0.f, TransformedSin()));
	}
}

