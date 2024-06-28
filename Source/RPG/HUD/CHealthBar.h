#pragma once
#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "CHealthBar.generated.h"

class UProgressBar;

UCLASS()
class RPG_API UCHealthBar : public UUserWidget
{
	GENERATED_BODY()

public:
	UPROPERTY(meta = (BindWidget))
	UProgressBar* HealthBar;//여기 변수이름(HealthBar)와 BP의 변수(HealthBar)는 일치해야만 한다.
};
