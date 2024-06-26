#pragma once
#include "CoreMinimal.h"
#include "Characters/CEnemy.h"
#include "CEnemy_AI.generated.h"

class UBehaviorTree;
class UCAIBehaviorComponent;
class ACPatrolPath;
class UWidgetComponent;

UCLASS()
class RPG_API ACEnemy_AI : public ACEnemy
{
	GENERATED_BODY()

public:
	ACEnemy_AI();
	virtual void Tick(float DeltaTime) override;
	void End_Hitted() override;//CEnemy의 End_Hitted 재정의

	FORCEINLINE uint8 GetTeamID() { return TeamID; }
	FORCEINLINE UBehaviorTree* GetBehaviorTree() { return BehaviorTree; }
	FORCEINLINE ACPatrolPath* GetPatrolPath() { return PatrolPath; }

protected:
	virtual void BeginPlay() override;
	void Hitted() override;//CEnemy의 Hitted 재정의

private:
	void UpdateLabelRenderScale();
	
	UPROPERTY(EditDefaultsOnly, Category = "AI")
	UBehaviorTree* BehaviorTree;

	UPROPERTY(EditDefaultsOnly, Category = "Label")
	float LabelViewAmount = 3000.0f;

	UPROPERTY(EditAnywhere, Category = "Patrol")
	ACPatrolPath* PatrolPath;//클래스 밖에서도 지정할 수 있어야 한다. 서로간 만들어진 객체를 참조할것이라서 softObjectPtr 사용

	UPROPERTY(VisibleDefaultsOnly)
	UWidgetComponent* LabelWidget;

	UPROPERTY(VisibleDefaultsOnly)
	UCAIBehaviorComponent* Behavior;
};
