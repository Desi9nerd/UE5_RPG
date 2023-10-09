#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "CAIBehaviorComponent.generated.h"

UENUM(BlueprintType)
enum class EAIStateType : uint8
{
	Wait = 0, Approach, Action, Patrol, Hitted, Avoid, Dead, Max,
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FAIStateTypeChanged, EAIStateType, InPrevType, EAIStateType, InNewType);

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class RPG_API UCAIBehaviorComponent : public UActorComponent
{
	GENERATED_BODY()
		
public:
	bool IsWaitMode();
	bool IsApproachMode();
	bool IsActionMode();
	bool IsPatrolMode();
	bool IsHittedMode();
	bool IsAvoidMode();
	bool IsDeadMode();
	
	UCAIBehaviorComponent();

	FORCEINLINE void SetBlackboard(class UBlackboardComponent* InBlackboard) { Blackboard = InBlackboard; }

	class ACharacter* GetTarget();

	FVector GetPatrolLocation();
	void SetPatrolLocation(const FVector& InLocation);
	FVector GetAvoidLocation();

	void SetWaitMode();
	void SetApproachMode();
	void SetActionMode();
	void SetPatrolMode();
	void SetHittedMode();
	void SetAvoidMode();
	void SetDeadMode();

protected:
	virtual void BeginPlay() override;

private:
	void ChangeType(EAIStateType InType);
	EAIStateType GetType();

public:
	FAIStateTypeChanged OnAIStateTypeChanged;

private:
	UPROPERTY(EditAnywhere, Category = "Key")
	FName AIStateTypeKey = "AIState";

	UPROPERTY(EditAnywhere, Category = "Key")
	FName TargetKey = "Target";

	UPROPERTY(EditAnywhere, Category = "Key")
	FName PatrolLocationKey = "Patrol_Location";

	UPROPERTY(EditAnywhere, Category = "Key")
	FName AvoidLocationKey = "Avoid_Location";

	class UBlackboardComponent* Blackboard;
};
