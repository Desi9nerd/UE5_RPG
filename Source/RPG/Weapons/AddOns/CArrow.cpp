#include "Weapons/AddOns/CArrow.h"
#include "Global.h"
#include "GameFramework/Character.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Components/CapsuleComponent.h"
#include "Kismet/GameplayStatics.h"

ACArrow::ACArrow()
{
	CHelpers::CreateComponent<UCapsuleComponent>(this, &Capsule, "Capsule");
	CHelpers::CreateActorComponent<UProjectileMovementComponent>(this, &Projectile, "Projectile");

	static ConstructorHelpers::FObjectFinder<UParticleSystem> Particle(TEXT("/Game/ParagonSparrow/FX/Particles/Sparrow/Abilities/Primary/FX/P_Sparrow_HitHero.P_Sparrow_HitHero"));
	ArrowParticle = Particle.Object;

	Projectile->ProjectileGravityScale = 1;
	Capsule->BodyInstance.bNotifyRigidBodyCollision = true;//BP와는 달리 UnrealC++에서는 bNotifyRigidBodyCollision를 true로 켜줘야 Block 연산이 일어난다.
	Capsule->SetCollisionProfileName("BlockAll");//Overlap과는 달리 Block의 경우 충돌이 일어나면 멈춘다. 화살은 적에게 박히면 멈춰야하기 때문에 Block으로 설정한다.
}

void ACArrow::BeginPlay()
{
	Super::BeginPlay();

	//화살이 Player에게 붙어있어야 하므로 충돌을 끄고 시작한다.
	Capsule->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	//추후에 화살 충돌이 일어나야 하므로 Delegate 연결을 해준다.
	Capsule->OnComponentHit.AddDynamic(this, &ACArrow::OnComponentHit);

	Projectile->Deactivate();//포물선 움직임이 필요없는 상태이므로 꺼준다.
}

void ACArrow::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);

	//소유자 이벤트를 콜해 자기 자신을 지워준다. FProjectileEndPlay OnEndPlay 사용하여 Delegate 이벤트 콜.
	if (OnEndPlay.IsBound())
		OnEndPlay.Broadcast(this);
}

void ACArrow::Shoot(const FVector& InForward)
{
	Projectile->Velocity = InForward * Projectile->InitialSpeed;
	Projectile->Activate();

	Capsule->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);//Collision을 켜준다.
}

void ACArrow::OnComponentHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	SetLifeSpan(LifeSpanAfterCollision);

	for (AActor* actor : Ignores)
		CheckTrue(actor == OtherActor);

	Projectile->StopMovementImmediately();
	Projectile->ProjectileGravityScale = 0;

	AttachToActor(OtherActor, FAttachmentTransformRules::KeepWorldTransform, NAME_None);

	Capsule->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	ACharacter* character = Cast<ACharacter>(OtherActor);
	if (!!character && OnHit.IsBound())
		OnHit.Broadcast(this, character);

	//충돌 위치에 Emitter 넣어주기
	if (ArrowParticle)
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ArrowParticle, GetActorLocation());
}