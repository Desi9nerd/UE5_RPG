#pragma once

#include "CoreMinimal.h"
#include "Weapons/CSubAction.h"
#include "Weapons/CWeaponStructures.h"
#include "CSubAction_Around.generated.h"

UCLASS(Blueprintable)
class RPG_API UCSubAction_Around : public UCSubAction
{
	GENERATED_BODY()

private:
	UPROPERTY(EditDefaultsOnly, Category = "FallObject")
		TSubclassOf<class ACThornObject> ObjectClass;

	UPROPERTY(EditDefaultsOnly, Category = "FallObject")
		FVector ObjectLocation;//Spawn시킬 위치

private:
	UPROPERTY(EditDefaultsOnly, Category = "Action")
		FDoActionData ActionData;

public:
	UCSubAction_Around();

public:
	virtual void Pressed() override;

	virtual void Begin_SubAction_Implementation() override;
	virtual void End_SubAction_Implementation() override;
};
