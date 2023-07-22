#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "CUserWidget_CrossHair.generated.h"

UCLASS()
class RPG_API UCUserWidget_CrossHair : public UUserWidget
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintImplementableEvent, Category = "CrossHair")
		void CrossHair_Red();

	UFUNCTION(BlueprintImplementableEvent, Category = "CrossHair")
		void CrossHair_White();

};
