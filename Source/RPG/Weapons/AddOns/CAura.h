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
	void OnSystemFinished(class UNiagaraComponent* PSystem);//Collision을 없애주는 역할의 함수

	//********************************************************
	//충돌 이벤트 2개
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
	UBoxComponent* Box;//나이아가라 Box 충돌체

	TArray<ACharacter*> Hitted;//Hitted되는 목록
	FTimerHandle TimerHandle;//Hitted 목록을 TimberHandle을 이용해서 데미지를 주기 위해 사용하는 변수
};
