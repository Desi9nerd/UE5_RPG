#pragma once
#include "CoreMinimal.h"
#include "BehaviorTree/BTService.h"
#include "CBTService_KillerClown.generated.h"

UCLASS()
class RPG_API UCBTService_KillerClown : public UBTService
{
	GENERATED_BODY()

public:
	UCBTService_KillerClown();

protected:
	void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;

private:
	UPROPERTY(EditAnywhere, Category = "Action")
	float ActionRange = 1200;
};
