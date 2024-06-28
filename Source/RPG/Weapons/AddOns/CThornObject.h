#pragma once
#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "NiagaraDataInterfaceExport.h"
#include "Weapons/CWeaponStructures.h"
#include "CThornObject.generated.h"

class UFXSystemAsset;

UCLASS()
class RPG_API ACThornObject
	: public AActor, public INiagaraParticleCallbackHandler
{
	GENERATED_BODY()

public:
	ACThornObject();
	void ReceiveParticleData_Implementation(const TArray<FBasicParticleData>& Data, UNiagaraSystem* NiagaraSystem, const FVector& SimulationPositionOffset) override;

protected:
	virtual void BeginPlay() override;
	
private:
	UFUNCTION()
	void OnSystemFinished(class UNiagaraComponent* PSystem);


	UPROPERTY(EditDefaultsOnly, Category = "Hit")
	FHitData HitData;

	UPROPERTY(EditDefaultsOnly, Category = "Niagara")
	UStaticMesh* NiagaraMesh;//나이아가라 매쉬

	UPROPERTY(EditDefaultsOnly, Category = "Niagara")
	FRotator NiagaraMeshRotation;//회전값

	UPROPERTY(EditDefaultsOnly, Category = "Collision")
	UFXSystemAsset* CollisionEffect;//충돌 했을때 이펙트

	UPROPERTY(EditDefaultsOnly, Category = "Collision")
	FTransform CollisionEffectTransform;//충돌했을때 보정 위치값

	UPROPERTY(VisibleDefaultsOnly)
	USceneComponent* Root;

	UPROPERTY(VisibleDefaultsOnly)
	UNiagaraComponent* Niagara;
	
	FVector BoxExtent;//Finished 되었을 때 계산할 Box
};
