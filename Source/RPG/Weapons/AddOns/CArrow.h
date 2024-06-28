#pragma once
#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CArrow.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FProjectileHit, class AActor*, InCauser, class ACharacter*, InOtherCharacter);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FProjectileEndPlay, class ACArrow*, InDestroyer);

class UCapsuleComponent;
class UProjectileMovementComponent;

UCLASS()
class RPG_API ACArrow : public AActor
{
	GENERATED_BODY()


public:
	FORCEINLINE void AddIgnoreActor(AActor* InActor) { Ignores.Add(InActor); }
	
	ACArrow();
	void Shoot(const FVector& InForward);
	
	FProjectileHit OnHit;
	FProjectileEndPlay OnEndPlay;
	
	UParticleSystem* ArrowParticle;

protected:
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;//공격될 때 화살을 소유하고 있는 목록에서 제거해준다.

private:
	UFUNCTION()
	void OnComponentHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);
	
	UPROPERTY(EditDefaultsOnly, Category = "LifeSpan")
	float LifeSpanAfterCollision = 3.f;//충돌 후 몇 초 후에 사라지는지

	UPROPERTY(VisibleDefaultsOnly)
	UCapsuleComponent* Capsule;//충돌체

	UPROPERTY(VisibleDefaultsOnly)
	UProjectileMovementComponent* Projectile;//Projectile

	TArray<AActor*> Ignores;
};
