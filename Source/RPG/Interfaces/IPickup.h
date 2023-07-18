#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "IPickup.generated.h"

UINTERFACE(MinimalAPI)
class UIPickup : public UInterface
{
	GENERATED_BODY()
};

class RPG_API IIPickup
{
	GENERATED_BODY()
		
public:
	virtual void SetOverlappingItem(class ACItem* Item);
	virtual void AddSouls(class ASoul* Soul);
	virtual void AddGold(class ATreasure* Treasure);
};
