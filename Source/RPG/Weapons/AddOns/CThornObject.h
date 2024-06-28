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
	UStaticMesh* NiagaraMesh;//���̾ư��� �Ž�

	UPROPERTY(EditDefaultsOnly, Category = "Niagara")
	FRotator NiagaraMeshRotation;//ȸ����

	UPROPERTY(EditDefaultsOnly, Category = "Collision")
	UFXSystemAsset* CollisionEffect;//�浹 ������ ����Ʈ

	UPROPERTY(EditDefaultsOnly, Category = "Collision")
	FTransform CollisionEffectTransform;//�浹������ ���� ��ġ��

	UPROPERTY(VisibleDefaultsOnly)
	USceneComponent* Root;

	UPROPERTY(VisibleDefaultsOnly)
	UNiagaraComponent* Niagara;
	
	FVector BoxExtent;//Finished �Ǿ��� �� ����� Box
};
