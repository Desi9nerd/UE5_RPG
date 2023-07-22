#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CArrow.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FProjectileHit, class AActor*, InCauser, class ACharacter*, InOtherCharacter);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FProjectileEndPlay, class ACArrow*, InDestroyer);

UCLASS()
class RPG_API ACArrow : public AActor
{
	GENERATED_BODY()

private:
	UPROPERTY(EditDefaultsOnly, Category = "LifeSpan")
		float LifeSpanAfterCollision = 1;//충돌 후 몇 초 후에 사라지는지

	UPROPERTY(VisibleDefaultsOnly)
		class UCapsuleComponent* Capsule;//충돌체

	UPROPERTY(VisibleDefaultsOnly)
		class UProjectileMovementComponent* Projectile;//Projectile

public:
	FORCEINLINE void AddIgnoreActor(AActor* InActor) { Ignores.Add(InActor); }

public:
	ACArrow();

protected:
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;//공격될 때 화살을 소유하고 있는 목록에서 제거해준다.

public:
	void Shoot(const FVector& InForward);

private:
	UFUNCTION()
		void OnComponentHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

public:
	FProjectileHit OnHit;
	FProjectileEndPlay OnEndPlay;

private:
	TArray<AActor*> Ignores;

public:
	UParticleSystem* ArrowParticle;
};
