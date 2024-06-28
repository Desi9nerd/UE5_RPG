#pragma once
#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Weapons/CWeaponStructures.h"
#include "NiagaraDataInterfaceExport.h"
#include "CAura.generated.h"

class USceneComponent;
class UNiagaraComponent;
class UBoxComponent;
class ACharacter;

UCLASS()
class RPG_API ACAura
	: public AActor, public INiagaraParticleCallbackHandler
{
	GENERATED_BODY()

public:
	ACAura();
	virtual void ReceiveParticleData_Implementation(const TArray<FBasicParticleData>& Data, UNiagaraSystem* NiagaraSystem, const FVector& SimulationPositionOffset) override;

protected:
	virtual void BeginPlay() override;

private:
	UFUNCTION()
	void OnSystemFinished(class UNiagaraComponent* PSystem);//Collision�� �����ִ� ������ �Լ�

	//********************************************************
	//�浹 �̺�Ʈ 2��
	UFUNCTION()
	void OnComponentBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	UFUNCTION()
	void OnComponentEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
	//********************************************************

	void OnApplyDamage();

	UPROPERTY(EditDefaultsOnly, Category = "Damage")
	FHitData HitData;

	UPROPERTY(EditDefaultsOnly, Category = "Damage")
	float DamageInterval = 0.1f;

	UPROPERTY(VisibleAnywhere)
	USceneComponent* Root;

	UPROPERTY(VisibleAnywhere)
	UNiagaraComponent* Niagara;

	UPROPERTY(VisibleAnywhere)
	UBoxComponent* Box;//���̾ư��� Box �浹ü

	TArray<ACharacter*> Hitted;//Hitted�Ǵ� ���
	FTimerHandle TimerHandle;//Hitted ����� TimberHandle�� �̿��ؼ� �������� �ֱ� ���� ����ϴ� ����
};
