#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "CHitNumber.generated.h"

UCLASS()
class RPG_API UCHitNumber : public UUserWidget
{
	GENERATED_BODY()

private:
	UPROPERTY(meta = (BindWidget))
		class UTextBlock* HitNumberText;
};
