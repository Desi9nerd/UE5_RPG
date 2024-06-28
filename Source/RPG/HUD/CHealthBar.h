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
	UProgressBar* HealthBar;//���� �����̸�(HealthBar)�� BP�� ����(HealthBar)�� ��ġ�ؾ߸� �Ѵ�.
};
