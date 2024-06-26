#pragma once
#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "IHit.generated.h"

UINTERFACE(MinimalAPI)
class UIHit : public UInterface
{
	GENERATED_BODY()
};

class RPG_API IIHit
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintNativeEvent)
	void GetHit(const FVector& ImpactPoint, AActor* Hitter);
};
