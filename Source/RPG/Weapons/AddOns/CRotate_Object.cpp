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

	//구형태에 가깝게 HalfHeight와 Radius값을 동일하게 설정하였다.
	Capsule->SetCapsuleHalfHeight(44);
	Capsule->SetCapsuleRadius(44);

	InitialLifeSpan = 5;

	HitData.Launch.X = 0;//HitData 초기값 설정.
	HitData.Power = 2;//HitData 초기값 설정.	
}

void ACRotate_Object::BeginPlay()
{
	Super::BeginPlay();

	Angle = UKismetMathLibrary::RandomFloatInRange(0, 360);//시작 각도를 랜덤으로 잡아준다. 매번 앞에서만 스폰되는것을 방지하기 위해서다.

	//충돌
	Capsule->OnComponentBeginOverlap.AddDynamic(this, &ACRotate_Object::OnComponentBeginOverlap);
	Capsule->OnComponentEndOverlap.AddDynamic(this, &ACRotate_Object::OnComponentEndOverlap);

	GetWorld()->GetTimerManager().SetTimer(TimerHandle, this, &ACRotate_Object::SendDamage, DamageInteval, true);//타이머 설정. 타이머가 돌고있는 동안 계속해서 Damage가 들어간다.
}

void ACRotate_Object::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);

	GetWorld()->GetTimerManager().ClearTimer(TimerHandle);//타이머 제거.
}

void ACRotate_Object::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);


	FVector location = GetOwner()->GetActorLocation();

	//시계방향인 경우와 반시계방향인 경우를 고려해서 삼항 연산자로 구현한다.
	Angle += (bNegative ? -Speed : +Speed) * DeltaTime;
	if (FMath::IsNearlyEqual(Angle, bNegative ? -360 : +360))
		Angle = 0;//짐벌락 현상을 방지하기 위해서 +-360도가 되었을때 0도로 만들어준다.

	FVector distance = FVector(Distance, 0, Height);
	FVector value = distance.RotateAngleAxis(Angle, FVector::UpVector);//UpVector(Yaw)기준으로 회전.
	location += value;//현재위치에서 회전해서 이동한 위치값을 더해준다.

	SetActorLocation(location);//위치 갱신
	SetActorRotation(FRotator(0, Angle, 0));//각 갱신
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
	//데미지 전달.
	for (int32 i = Hitted.Num() - 1; i >= 0; i--)
		HitData.SendDamage(Cast<ACharacter>(GetOwner()), this, Hitted[i]);
}