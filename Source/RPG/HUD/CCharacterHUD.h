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
	//const�� ����Ͽ� return value�������� ����.
	FORCEINLINE UCPlayerOverlay* GetPlayerOverlay() const { return PlayerOverlay; }
};
