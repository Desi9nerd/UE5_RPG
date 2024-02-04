#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "CAIController.generated.h"

UCLASS()
class RPG_API ACAIController : public AAIController
{
	GENERATED_BODY()
		
public:
	ACAIController();

protected:
	virtual void BeginPlay() override;
	void OnPossess(APawn* InPawn) override;
	void OnUnPossess() override;

private:
	UFUNCTION()
	void OnPerceptionUpdated(const TArray<AActor*>& UpdatedActors);
	
	UPROPERTY(VisibleAnywhere)
	class UAIPerceptionComponent* Perception;

	class ACEnemy_AI* Enemy;
	class UCAIBehaviorComponent* Behavior;
	class UAISenseConfig_Sight* Sight;//시야 감지
};
