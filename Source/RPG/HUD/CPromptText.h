#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "CPromptText.generated.h"

UCLASS()
class RPG_API UCPromptText : public UUserWidget
{
	GENERATED_BODY()
	
private:
	UPROPERTY(meta = (BindWidget))
		class UTextBlock* PromptText;
};
