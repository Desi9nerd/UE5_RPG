#pragma once

#include "CoreMinimal.h"
#include "Components/WidgetComponent.h"
#include "CHealthBarComponent.generated.h"

UCLASS()
class RPG_API UCHealthBarComponent : public UWidgetComponent
{
	GENERATED_BODY()

public:
	void SetHealthPercent(float Percent);

private:
	UPROPERTY()
	class UCHealthBar* HealthBarWidget;
};
