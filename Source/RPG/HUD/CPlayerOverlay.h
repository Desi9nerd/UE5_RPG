#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "CPlayerOverlay.generated.h"

UCLASS()
class RPG_API UCPlayerOverlay : public UUserWidget
{
	GENERATED_BODY()

public:
	void SetHealthBarPercent(float Percent);//HP Bar����
	void SetStaminaBarPercent(float Percent);//Stamina Bar����
	void SetGold(int32 Gold);//Gold ����
	void SetSouls(int32 Souls);//Soul ����

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
