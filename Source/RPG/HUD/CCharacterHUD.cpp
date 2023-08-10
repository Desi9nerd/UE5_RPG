#include "HUD/CCharacterHUD.h"
#include "CPlayerOverlay.h"
#include "CPromptText.h"

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
		if (Controller && PromptTextClass)
		{
			PromptText = CreateWidget<UCPromptText>(Controller, PromptTextClass);
			PromptText->AddToViewport();
			PromptText->SetVisibility(ESlateVisibility::Hidden);
		}
	}
}

void ACCharacterHUD::OnStaminaPromptText()
{
	PromptText->SetVisibility(ESlateVisibility::Visible);

	//FTimerDelegate delegate = FTimerDelegate::CreateLambda([this]() { OffStaminaPromptText(); });
	
	//GetWorld()->GetTimerManager().ClearTimer(TimerHandle);
	//GetWorld()->GetTimerManager().SetTimer(TimerHandle, delegate, 3.0f, false, 0);
}

void ACCharacterHUD::OffStaminaPromptText()
{
	GetWorld()->GetTimerManager().ClearTimer(TimerHandle);

	PromptText->SetVisibility(ESlateVisibility::Hidden);
}
