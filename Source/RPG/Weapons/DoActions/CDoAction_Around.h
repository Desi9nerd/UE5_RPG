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
	TArray<TSubclassOf<class ACRotate_Object>> RotateClasses;//회전체를 여러개 만들기위해 배열로 생성한다.
};