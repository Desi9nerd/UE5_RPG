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
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason);//Ÿ�̸Ӹ� �����Ű�� ���� EndPlay�� �ʿ��ϴ�.
	
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
	float Speed = 300.f;//���ư��� �ӵ�

	UPROPERTY(EditDefaultsOnly, Category = "Spawn")
	float Height = 0.f;//�����Ǵ� ����

	UPROPERTY(EditDefaultsOnly, Category = "Spawn")
	float Distance = 150.f;//�߽����κ����� �Ÿ�

	UPROPERTY(EditDefaultsOnly, Category = "Spawn")
	bool bNegative;//�ð����, �ݽð� ���� ����

	UPROPERTY(EditDefaultsOnly, Category = "Spawn")
	float DamageInteval = 0.1f;//������ �� ����

	UPROPERTY(VisibleDefaultsOnly)
	UCapsuleComponent* Capsule;//�浹ü

	UPROPERTY(VisibleDefaultsOnly)
	UParticleSystemComponent* Particle;

	float Angle;//���� ���ư��� ����

	TArray<ACharacter*> Hitted;//������ ���� �ֵ�
	FTimerHandle TimerHandle;//Ÿ�̸�
};
