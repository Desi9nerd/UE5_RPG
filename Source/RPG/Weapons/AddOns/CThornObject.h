#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "NiagaraDataInterfaceExport.h"
#include "Weapons/CWeaponStructures.h"
#include "CThornObject.generated.h"

UCLASS()
class RPG_API ACThornObject
	: public AActor, public INiagaraParticleCallbackHandler
{
	GENERATED_BODY()

private:
	UPROPERTY(EditDefaultsOnly, Category = "Hit")
		FHitData HitData;

	UPROPERTY(EditDefaultsOnly, Category = "Niagara")
		class UStaticMesh* NiagaraMesh;//나이아가라 매쉬

	UPROPERTY(EditDefaultsOnly, Category = "Niagara")
		FRotator NiagaraMeshRotation;//회전값

	UPROPERTY(EditDefaultsOnly, Category = "Collision")
		class UFXSystemAsset* CollisionEffect;//충돌 했을때 이펙트

	UPROPERTY(EditDefaultsOnly, Category = "Collision")
		FTransform CollisionEffectTransform;//충돌했을때 보정 위치값

private:
	UPROPERTY(VisibleDefaultsOnly)
		class USceneComponent* Root;

	UPROPERTY(VisibleDefaultsOnly)
		class UNiagaraComponent* Niagara;

public:
	ACThornObject();

protected:
	virtual void BeginPlay() override;

public:
	void ReceiveParticleData_Implementation(const TArray<FBasicParticleData>& Data, UNiagaraSystem* NiagaraSystem, const FVector& SimulationPositionOffset) override;

private:
	UFUNCTION()
		void OnSystemFinished(class UNiagaraComponent* PSystem);

private:
	FVector BoxExtent;//Finished 되었을 때 계산할 Box
};
