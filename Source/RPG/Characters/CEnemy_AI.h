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
	void End_Hitted() override;//CEnemy�� End_Hitted ������

	FORCEINLINE uint8 GetTeamID() { return TeamID; }
	FORCEINLINE UBehaviorTree* GetBehaviorTree() { return BehaviorTree; }
	FORCEINLINE ACPatrolPath* GetPatrolPath() { return PatrolPath; }

protected:
	virtual void BeginPlay() override;
	void Hitted() override;//CEnemy�� Hitted ������

private:
	void UpdateLabelRenderScale();
	
	UPROPERTY(EditDefaultsOnly, Category = "AI")
	UBehaviorTree* BehaviorTree;

	UPROPERTY(EditDefaultsOnly, Category = "Label")
	float LabelViewAmount = 3000.0f;

	UPROPERTY(EditAnywhere, Category = "Patrol")
	ACPatrolPath* PatrolPath;//Ŭ���� �ۿ����� ������ �� �־�� �Ѵ�. ���ΰ� ������� ��ü�� �����Ұ��̶� softObjectPtr ���

	UPROPERTY(VisibleDefaultsOnly)
	UWidgetComponent* LabelWidget;

	UPROPERTY(VisibleDefaultsOnly)
	UCAIBehaviorComponent* Behavior;
};
