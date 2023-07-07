#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "CPlayerOverlay.generated.h"

UCLASS()
class RPG_API UCPlayerOverlay : public UUserWidget
{
	GENERATED_BODY()

public:
	void SetHealthBarPercent(float Percent);//HP Bar설정
	void SetStaminaBarPercent(float Percent);//Stamina Bar설정
	void SetGold(int32 Gold);//Gold 설정
	void SetSouls(int32 Souls);//Soul 설정

private:
	UPROPERTY(meta = (BindWidget))
		class UProgressBar* HealthProgressBar;

	UPROPERTY(meta = (BindWidget))
		class UProgressBar* StaminaProgressBar;

	UPROPERTY(meta = (BindWidget))
		class UTextBlock* GoldText;

	UPROPERTY(meta = (BindWidget))
		class UTextBlock* SoulsText;
};
