#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "CHealthBar.generated.h"

UCLASS()
class RPG_API UCHealthBar : public UUserWidget
{
	GENERATED_BODY()

public:
	UPROPERTY(meta = (BindWidget))
	class UProgressBar* HealthBar;//���� �����̸�(HealthBar)�� BP�� ����(HealthBar)�� ��ġ�ؾ߸� �Ѵ�.
};
