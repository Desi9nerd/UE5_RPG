#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Weapons/CWeaponStructures.h"
#include "CRotate_Object.generated.h"

UCLASS()
class RPG_API ACRotate_Object : public AActor
{
	GENERATED_BODY()

private:
	UPROPERTY(EditDefaultsOnly, Category = "Damage")
		FHitData HitData;

private:
	UPROPERTY(EditDefaultsOnly, Category = "Spawn")
		float Speed = 300.0f;//돌아가는 속도

	UPROPERTY(EditDefaultsOnly, Category = "Spawn")
		float Distance = 150.0f;//중심으로부터의 거리

	UPROPERTY(EditDefaultsOnly, Category = "Spawn")
		bool bNegative;//시계방향, 반시계 방향 결정

	UPROPERTY(EditDefaultsOnly, Category = "Spawn")
		float DamageInteval = 0.1f;//데미지 들어갈 간격

private:
	UPROPERTY(VisibleDefaultsOnly)
		class UCapsuleComponent* Capsule;//충돌체

	UPROPERTY(VisibleDefaultsOnly)
		class UParticleSystemComponent* Particle;

private:
	UFUNCTION()
		void OnComponentBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
		void OnComponentEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	UFUNCTION()
		void SendDamage();

public:
	ACRotate_Object();

protected:
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason);//타이머를 종료시키기 위해 EndPlay가 필요하다.

public:
	virtual void Tick(float DeltaTime) override;

private:
	float Angle;//현재 돌아가는 각도

	TArray<ACharacter*> Hitted;//데미지 받을 애들
	FTimerHandle TimerHandle;//타이머
};
