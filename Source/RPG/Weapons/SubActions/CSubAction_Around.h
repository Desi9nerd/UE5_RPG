#pragma once
#include "CoreMinimal.h"
#include "Weapons/CSubAction.h"
#include "Weapons/CWeaponStructures.h"
#include "CSubAction_Around.generated.h"

class ACThornObject;

UCLASS(Blueprintable)
class RPG_API UCSubAction_Around : public UCSubAction
{
	GENERATED_BODY()

public:
	virtual void Pressed() override;

	virtual void Begin_SubAction_Implementation() override;
	virtual void End_SubAction_Implementation() override;

private:
	UPROPERTY(EditDefaultsOnly, Category = "FallObject")
	TSubclassOf<ACThornObject> ObjectClass;

	UPROPERTY(EditDefaultsOnly, Category = "FallObject")
	FVector ObjectLocation;//Spawn시킬 위치

	UPROPERTY(EditDefaultsOnly, Category = "Action")
	FDoActionData ActionData;
};
