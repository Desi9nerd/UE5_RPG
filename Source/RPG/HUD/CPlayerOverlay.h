#pragma once
#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "CPlayerOverlay.generated.h"

class UProgressBar;

UCLASS()
class RPG_API UCPlayerOverlay : public UUserWidget
{
	GENERATED_BODY()

public:
	void SetHealthBarPercent(float Percent);//HP Bar설정
	void SetStaminaBarPercent(float Percent);//Stamina Bar설정

private:
	UPROPERTY(meta = (BindWidget))
	UProgressBar* HealthProgressBar;

	UPROPERTY(meta = (BindWidget))
	UProgressBar* StaminaProgressBar;
};
