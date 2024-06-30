#pragma once
#include "CoreMinimal.h"
#include "Weapons/CSubAction.h"
#include "Weapons/CWeaponStructures.h"
#include "CSubAction_Hammer.generated.h"

class ACAura;
class ACGhostTrail;

UCLASS(Blueprintable)
class RPG_API UCSubAction_Hammer : public UCSubAction
{
	GENERATED_BODY()
		
public:
	void Pressed() override;

	void Begin_SubAction_Implementation() override;
	void End_SubAction_Implementation() override;

private:
	UPROPERTY(EditDefaultsOnly, Category = "Aura")
	TSubclassOf<ACAura> AuraClass;

	UPROPERTY(EditDefaultsOnly, Category = "Aura")
	FVector AuraLoction;//Aura를 Spawn시킬 위치 변수

	UPROPERTY(EditDefaultsOnly, Category = "Action")
	FDoActionData ActionData;

	UPROPERTY(EditDefaultsOnly, Category = "Add-On")
	TSubclassOf<ACGhostTrail> GhostTrailClass;
	
	ACGhostTrail* GhostTrail;
};
