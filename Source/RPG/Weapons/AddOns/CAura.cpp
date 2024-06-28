#include "Weapons/AddOns/CAura.h"
#include "Global.h"
#include "NiagaraComponent.h"
#include "GameFramework/Character.h"
#include "Components/BoxComponent.h"

ACAura::ACAura()
{
	CHelpers::CreateComponent<USceneComponent>(this, &Root, TEXT("Root"));
	CHelpers::CreateComponent<UNiagaraComponent>(this, &Niagara, TEXT("Niagara"), Root);
	CHelpers::CreateComponent<UBoxComponent>(this, &Box, TEXT("Box"), Root);

	UNiagaraSystem* niagara;
	CHelpers::GetAsset<UNiagaraSystem>(&niagara, "NiagaraSystem'/Game/Assets/sA_StylizedSwordSet/Fx/NS_Ulti_lv1.NS_Ulti_lv1'");//나이아가라 에셋 할당.
	Niagara->SetAsset(niagara);
}

void ACAura::BeginPlay()
{
	Super::BeginPlay();

	Niagara->SetNiagaraVariableObject("Mesh_Scale", this);//Niagara 안에서 지정한 이름 "Mesh_Scale" 사용.
	Niagara->OnSystemFinished.AddDynamic(this, &ACAura::OnSystemFinished);

	Box->OnComponentBeginOverlap.AddDynamic(this, &ACAura::OnComponentBeginOverlap);
	Box->OnComponentEndOverlap.AddDynamic(this, &ACAura::OnComponentEndOverlap);

	//타이머 설정. 일정시간 동안 Damage를 준다.
	FTimerDelegate delegate = FTimerDelegate::CreateUObject(this, &ACAura::OnApplyDamage);
	GetWorld()->GetTimerManager().SetTimer(TimerHandle, delegate, DamageInterval, true, 0.f);
}

void ACAura::OnApplyDamage()
{
	for (int32 i = Hitted.Num() - 1; i >= 0; i--)
	{
		HitData.SendDamage(Cast<ACharacter>(GetOwner()), this, Hitted[i]);
	}
}

void ACAura::OnSystemFinished(UNiagaraComponent* PSystem)
{
	GetWorld()->GetTimerManager().ClearTimer(TimerHandle);//끝날 때 TimerHandle 제거

	Destroy();//Box 충돌체 소멸
}

void ACAura::ReceiveParticleData_Implementation(const TArray<FBasicParticleData>& Data, UNiagaraSystem* NiagaraSystem, const FVector& SimulationPositionOffset)
{
	Box->SetRelativeScale3D(Data[0].Position);


	FVector location = Box->GetScaledBoxExtent();
	location.Y = 0;//Y=0을 만든 이유는? 캐릭터 기준에서 기준선에 맞추기 위해서이다. 캐릭터 중앙 기준은 Y=0이다.

	CLog::Log(location);

	Box->SetRelativeLocation(location);
}

void ACAura::OnComponentBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	CheckTrue(GetOwner() == OtherActor);

	ACharacter* character = Cast<ACharacter>(OtherActor);
	if (IsValid(character))
	{
		Hitted.AddUnique(character);
	}
}

void ACAura::OnComponentEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	CheckTrue(GetOwner() == OtherActor);

	ACharacter* character = Cast<ACharacter>(OtherActor);
	if (IsValid(character))
	{
		Hitted.Remove(character);
	}
}
