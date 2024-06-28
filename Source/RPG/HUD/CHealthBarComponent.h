#pragma once
#include "CoreMinimal.h"
#include "Components/WidgetComponent.h"
#include "CHealthBarComponent.generated.h"

class UCHealthBar;

UCLASS()
class RPG_API UCHealthBarComponent : public UWidgetComponent
{
	GENERATED_BODY()

public:
	void SetHealthPercent(float Percent);

private:
	UPROPERTY()
	UCHealthBar* HealthBarWidget;
};
