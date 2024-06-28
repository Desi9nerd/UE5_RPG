#pragma once
#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Weapons/CWeaponStructures.h"
#include "CRotate_Object.generated.h"

class UCapsuleComponent;
class UParticleSystemComponent;

UCLASS()
class RPG_API ACRotate_Object : public AActor
{
	GENERATED_BODY()


public:
	ACRotate_Object();
	virtual void Tick(float DeltaTime) override;

protected:
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason);//타이머를 종료시키기 위해 EndPlay가 필요하다.
	
private:
	UFUNCTION()
	void OnComponentBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	UFUNCTION()
	void OnComponentEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	UFUNCTION()
	void SendDamage();


	UPROPERTY(EditDefaultsOnly, Category = "Damage")
	FHitData HitData;

	UPROPERTY(EditDefaultsOnly, Category = "Spawn")
	float Speed = 300.f;//돌아가는 속도

	UPROPERTY(EditDefaultsOnly, Category = "Spawn")
	float Height = 0.f;//스폰되는 높이

	UPROPERTY(EditDefaultsOnly, Category = "Spawn")
	float Distance = 150.f;//중심으로부터의 거리

	UPROPERTY(EditDefaultsOnly, Category = "Spawn")
	bool bNegative;//시계방향, 반시계 방향 결정

	UPROPERTY(EditDefaultsOnly, Category = "Spawn")
	float DamageInteval = 0.1f;//데미지 들어갈 간격

	UPROPERTY(VisibleDefaultsOnly)
	UCapsuleComponent* Capsule;//충돌체

	UPROPERTY(VisibleDefaultsOnly)
	UParticleSystemComponent* Particle;

	float Angle;//현재 돌아가는 각도

	TArray<ACharacter*> Hitted;//데미지 받을 애들
	FTimerHandle TimerHandle;//타이머
};
