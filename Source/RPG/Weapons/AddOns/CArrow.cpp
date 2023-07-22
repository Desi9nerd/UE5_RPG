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
	Capsule->BodyInstance.bNotifyRigidBodyCollision = true;//BP�ʹ� �޸� UnrealC++������ bNotifyRigidBodyCollision�� true�� ����� Block ������ �Ͼ��.
	Capsule->SetCollisionProfileName("BlockAll");//Overlap���� �޸� Block�� ��� �浹�� �Ͼ�� �����. ȭ���� ������ ������ ������ϱ� ������ Block���� �����Ѵ�.
}

void ACArrow::BeginPlay()
{
	Super::BeginPlay();

	//ȭ���� Player���� �پ��־�� �ϹǷ� �浹�� ���� �����Ѵ�.
	Capsule->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	//���Ŀ� ȭ�� �浹�� �Ͼ�� �ϹǷ� Delegate ������ ���ش�.
	Capsule->OnComponentHit.AddDynamic(this, &ACArrow::OnComponentHit);

	Projectile->Deactivate();//������ �������� �ʿ���� �����̹Ƿ� ���ش�.
}

void ACArrow::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);

	//������ �̺�Ʈ�� ���� �ڱ� �ڽ��� �����ش�. FProjectileEndPlay OnEndPlay ����Ͽ� Delegate �̺�Ʈ ��.
	if (OnEndPlay.IsBound())
		OnEndPlay.Broadcast(this);
}

void ACArrow::Shoot(const FVector& InForward)
{
	Projectile->Velocity = InForward * Projectile->InitialSpeed;
	Projectile->Activate();

	Capsule->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);//Collision�� ���ش�.
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

	//�浹 ��ġ�� Emitter �־��ֱ�
	if (ArrowParticle)
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ArrowParticle, GetActorLocation());
}