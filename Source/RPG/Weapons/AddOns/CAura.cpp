#include "Weapons/AddOns/CAura.h"
#include "Global.h"
#include "NiagaraComponent.h"
#include "GameFramework/Character.h"
#include "Components/BoxComponent.h"

ACAura::ACAura()
{
	CHelpers::CreateComponent<USceneComponent>(this, &Root, "Root");
	CHelpers::CreateComponent<UNiagaraComponent>(this, &Niagara, "Niagara", Root);
	CHelpers::CreateComponent<UBoxComponent>(this, &Box, "Box", Root);

	UNiagaraSystem* niagara;
	CHelpers::GetAsset<UNiagaraSystem>(&niagara, "NiagaraSystem'/Game/Assets/sA_StylizedSwordSet/Fx/NS_Ulti_lv1.NS_Ulti_lv1'");//���̾ư��� ���� �Ҵ�.
	Niagara->SetAsset(niagara);
}

void ACAura::BeginPlay()
{
	Super::BeginPlay();

	Niagara->SetNiagaraVariableObject("Mesh_Scale", this);//Niagara �ȿ��� ������ �̸� "Mesh_Scale" ���.
	Niagara->OnSystemFinished.AddDynamic(this, &ACAura::OnSystemFinished);

	Box->OnComponentBeginOverlap.AddDynamic(this, &ACAura::OnComponentBeginOverlap);
	Box->OnComponentEndOverlap.AddDynamic(this, &ACAura::OnComponentEndOverlap);

	//Ÿ�̸� ����. �����ð� ���� Damage�� �ش�. 
	FTimerDelegate delegate = FTimerDelegate::CreateLambda([&]()
		{
			for (int32 i = Hitted.Num() - 1; i >= 0; i--)
				HitData.SendDamage(Cast<ACharacter>(GetOwner()), this, Hitted[i]);
		});

	GetWorld()->GetTimerManager().SetTimer(TimerHandle, delegate, DamageInterval, true, 0);
}

void ACAura::OnSystemFinished(UNiagaraComponent* PSystem)
{
	GetWorld()->GetTimerManager().ClearTimer(TimerHandle);//���� �� TimerHandle ����

	Destroy();//Box �浹ü �Ҹ�
}

void ACAura::ReceiveParticleData_Implementation(const TArray<FBasicParticleData>& Data, UNiagaraSystem* NiagaraSystem)
{
	Box->SetRelativeScale3D(Data[0].Position);


	FVector location = Box->GetScaledBoxExtent();
	location.Y = 0;//Y=0�� ���� ������? ĳ���� ���ؿ��� ���ؼ��� ���߱� ���ؼ��̴�. ĳ���� �߾� ������ Y=0�̴�.

	CLog::Log(location);

	Box->SetRelativeLocation(location);
}

void ACAura::OnComponentBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	CheckTrue(GetOwner() == OtherActor);

	ACharacter* character = Cast<ACharacter>(OtherActor);
	if (!!character)
		Hitted.AddUnique(character);
}

void ACAura::OnComponentEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	CheckTrue(GetOwner() == OtherActor);

	ACharacter* character = Cast<ACharacter>(OtherActor);
	if (!!character)
		Hitted.Remove(character);
}