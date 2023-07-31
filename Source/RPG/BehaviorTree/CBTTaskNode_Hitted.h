#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "CBTTaskNode_Hitted.generated.h"

UCLASS()
class RPG_API UCBTTaskNode_Hitted : public UBTTaskNode
{
	GENERATED_BODY()

public:
	UCBTTaskNode_Hitted();

protected:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	virtual void TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
};
