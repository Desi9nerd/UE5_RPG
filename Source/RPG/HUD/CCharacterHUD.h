#pragma once

#include "CoreMinimal.h"
#include "CPromptText.h"
#include "GameFramework/HUD.h"
#include "CCharacterHUD.generated.h"

class UCPlayerOverlay;
class UCPromptText;

UCLASS()
class RPG_API ACCharacterHUD : public AHUD
{
	GENERATED_BODY()

protected:
	virtual void BeginPlay() override;

private:
	UPROPERTY(EditDefaultsOnly, Category = PlayerAttributeHUD)
	TSubclassOf<UCPlayerOverlay> PlayerOverlayClass;

	UPROPERTY(EditDefaultsOnly, Category = PromptText)
	TSubclassOf<UCPromptText> PromptTextClass;

	UPROPERTY()
	UCPlayerOverlay* PlayerOverlay;

	UPROPERTY()
	UCPromptText* PromptText;

public:
	//const를 사용하여 return value전용으로 만듬.
	FORCEINLINE UCPlayerOverlay* GetPlayerOverlay() const { return PlayerOverlay; }

	void OnStaminaPromptText();
	void OffStaminaPromptText();

private:
	FTimerHandle TimerHandle;
};
