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
		float Speed = 300.0f;//���ư��� �ӵ�

	UPROPERTY(EditDefaultsOnly, Category = "Spawn")
		float Distance = 150.0f;//�߽����κ����� �Ÿ�

	UPROPERTY(EditDefaultsOnly, Category = "Spawn")
		bool bNegative;//�ð����, �ݽð� ���� ����

	UPROPERTY(EditDefaultsOnly, Category = "Spawn")
		float DamageInteval = 0.1f;//������ �� ����

private:
	UPROPERTY(VisibleDefaultsOnly)
		class UCapsuleComponent* Capsule;//�浹ü

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
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason);//Ÿ�̸Ӹ� �����Ű�� ���� EndPlay�� �ʿ��ϴ�.

public:
	virtual void Tick(float DeltaTime) override;

private:
	float Angle;//���� ���ư��� ����

	TArray<ACharacter*> Hitted;//������ ���� �ֵ�
	FTimerHandle TimerHandle;//Ÿ�̸�
};
