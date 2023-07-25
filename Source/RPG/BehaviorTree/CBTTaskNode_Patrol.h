#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "CBTTaskNode_Patrol.generated.h"

UCLASS()
class RPG_API UCBTTaskNode_Patrol : public UBTTaskNode
{
	GENERATED_BODY()

private:
    UPROPERTY(EditAnywhere, Category = "Patrol")
        bool bDebugMode;//디버그 모드 on/off

    UPROPERTY(EditAnywhere, Category = "Patrol")
        float AcceptanceDistance = 20;//순찰 경로 간격

    UPROPERTY(EditAnywhere, Category = "Random")
        float RandomRadius = 1500;//순찰 경로가 없을 때 랜덤으로 움직이는 범위

public:
    UCBTTaskNode_Patrol();

protected:
    virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
    virtual void TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;

};
