#pragma once
#include "CoreMinimal.h"
#include "BehaviorTree/BTService.h"
#include "CBTService_Range.generated.h"

UCLASS()
class RPG_API UCBTService_Range : public UBTService
{
	GENERATED_BODY()

public:
	UCBTService_Range();

protected:
	void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;

private:
	UPROPERTY(EditAnywhere, Category = "Action")
	float AvoidRange = 500.f;//탐지되었을때 회피범위

	UPROPERTY(EditAnywhere, Category = "Action")
	bool bDrawDebug;//디버깅 그리기 회피범위
};
