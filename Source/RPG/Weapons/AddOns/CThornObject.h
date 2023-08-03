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
		class UStaticMesh* NiagaraMesh;//���̾ư��� �Ž�

	UPROPERTY(EditDefaultsOnly, Category = "Niagara")
		FRotator NiagaraMeshRotation;//ȸ����

	UPROPERTY(EditDefaultsOnly, Category = "Collision")
		class UFXSystemAsset* CollisionEffect;//�浹 ������ ����Ʈ

	UPROPERTY(EditDefaultsOnly, Category = "Collision")
		FTransform CollisionEffectTransform;//�浹������ ���� ��ġ��

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
	FVector BoxExtent;//Finished �Ǿ��� �� ����� Box
};
