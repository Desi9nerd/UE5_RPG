#include "Weapons/AddOns/CRotate_Object.h"
#include "Global.h"
#include "GameFramework/Character.h"
#include "Components/CapsuleComponent.h"
#include "Particles/ParticleSystemComponent.h"

ACRotate_Object::ACRotate_Object()
{
	PrimaryActorTick.bCanEverTick = true;

	CHelpers::CreateComponent<UCapsuleComponent>(this, &Capsule, "Capsule");
	CHelpers::CreateComponent<UParticleSystemComponent>(this, &Particle, "Particle", Capsule);

	//�����¿� ������ HalfHeight�� Radius���� �����ϰ� �����Ͽ���.
	Capsule->SetCapsuleHalfHeight(44);
	Capsule->SetCapsuleRadius(44);

	InitialLifeSpan = 5;

	HitData.Launch.X = 0;//HitData �ʱⰪ ����.
	HitData.Power = 2;//HitData �ʱⰪ ����.	
}

void ACRotate_Object::BeginPlay()
{
	Super::BeginPlay();

	Angle = UKismetMathLibrary::RandomFloatInRange(0, 360);//���� ������ �������� ����ش�. �Ź� �տ����� �����Ǵ°��� �����ϱ� ���ؼ���.

	//�浹
	Capsule->OnComponentBeginOverlap.AddDynamic(this, &ACRotate_Object::OnComponentBeginOverlap);
	Capsule->OnComponentEndOverlap.AddDynamic(this, &ACRotate_Object::OnComponentEndOverlap);

	GetWorld()->GetTimerManager().SetTimer(TimerHandle, this, &ACRotate_Object::SendDamage, DamageInteval, true);//Ÿ�̸� ����. Ÿ�̸Ӱ� �����ִ� ���� ����ؼ� Damage�� ����.
}

void ACRotate_Object::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);

	GetWorld()->GetTimerManager().ClearTimer(TimerHandle);//Ÿ�̸� ����.
}

void ACRotate_Object::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);


	FVector location = GetOwner()->GetActorLocation();

	//�ð������ ���� �ݽð������ ��츦 ����ؼ� ���� �����ڷ� �����Ѵ�.
	Angle += (bNegative ? -Speed : +Speed) * DeltaTime;
	if (FMath::IsNearlyEqual(Angle, bNegative ? -360 : +360))
		Angle = 0;//������ ������ �����ϱ� ���ؼ� +-360���� �Ǿ����� 0���� ������ش�.

	FVector distance = FVector(Distance, 0, Height);
	FVector value = distance.RotateAngleAxis(Angle, FVector::UpVector);//UpVector(Yaw)�������� ȸ��.
	location += value;//������ġ���� ȸ���ؼ� �̵��� ��ġ���� �����ش�.

	SetActorLocation(location);//��ġ ����
	SetActorRotation(FRotator(0, Angle, 0));//�� ����
}

void ACRotate_Object::OnComponentBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	CheckTrue(GetOwner() == OtherActor);

	ACharacter* character = Cast<ACharacter>(OtherActor);
	if (!!character)
		Hitted.AddUnique(character);
}

void ACRotate_Object::OnComponentEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	CheckTrue(GetOwner() == OtherActor);

	ACharacter* character = Cast<ACharacter>(OtherActor);
	if (!!character)
		Hitted.Remove(character);
}

void ACRotate_Object::SendDamage()
{
	//������ ����.
	for (int32 i = Hitted.Num() - 1; i >= 0; i--)
		HitData.SendDamage(Cast<ACharacter>(GetOwner()), this, Hitted[i]);
}