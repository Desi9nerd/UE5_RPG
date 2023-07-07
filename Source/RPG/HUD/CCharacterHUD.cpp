#include "HUD/CCharacterHUD.h"

#include "CPlayerOverlay.h"

void ACCharacterHUD::BeginPlay()
{
	Super::BeginPlay();

	UWorld* World = GetWorld();
	if (World)
	{
		APlayerController* Controller = World->GetFirstPlayerController();
		if (Controller && PlayerOverlayClass)
		{
			PlayerOverlay = CreateWidget<UCPlayerOverlay>(Controller, PlayerOverlayClass);
			PlayerOverlay->AddToViewport();
		}
	}
}
