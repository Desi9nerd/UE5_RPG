#pragma once
#include "CoreMinimal.h"
#include "Weapons/CDoAction.h"
#include "CDoAction_Around.generated.h"

UCLASS(Blueprintable)
class RPG_API UCDoAction_Around : public UCDoAction
{
	GENERATED_BODY()

public:
	void DoAction() override;
	void Begin_DoAction() override;

private:
	UPROPERTY(EditAnywhere, Category = "SpawnClass")
	TArray<TSubclassOf<class ACRotate_Object>> RotateClasses;//ȸ��ü�� ������ ��������� �迭�� �����Ѵ�.
};