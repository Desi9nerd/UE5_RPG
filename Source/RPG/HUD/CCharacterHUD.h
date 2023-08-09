#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "CCharacterHUD.generated.h"

class UCPlayerOverlay;

UCLASS()
class RPG_API ACCharacterHUD : public AHUD
{
	GENERATED_BODY()

protected:
	virtual void BeginPlay() override;

private:
	UPROPERTY(EditDefaultsOnly, Category = Slash)
		TSubclassOf<UCPlayerOverlay> PlayerOverlayClass;

	UPROPERTY()
		UCPlayerOverlay* PlayerOverlay;
public:
	//const를 사용하여 return value전용으로 만듬.
	FORCEINLINE UCPlayerOverlay* GetPlayerOverlay() const { return PlayerOverlay; }
};
