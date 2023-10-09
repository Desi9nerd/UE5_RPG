#pragma once

#include "CoreMinimal.h"
#include "Characters/CEnemy.h"
#include "CEnemy_AI.generated.h"

UCLASS()
class RPG_API ACEnemy_AI : public ACEnemy
{
	GENERATED_BODY()

public:
	ACEnemy_AI();
	virtual void Tick(float DeltaTime) override;
	void End_Hitted() override;//CEnemy�� End_Hitted ������

	FORCEINLINE uint8 GetTeamID() { return TeamID; }
	FORCEINLINE class UBehaviorTree* GetBehaviorTree() { return BehaviorTree; }
	FORCEINLINE class ACPatrolPath* GetPatrolPath() { return PatrolPath; }

protected:
	virtual void BeginPlay() override;
	void Hitted() override;//CEnemy�� Hitted ������

private:
	void UpdateLabelRenderScale();

private:
	UPROPERTY(EditDefaultsOnly, Category = "AI")
	class UBehaviorTree* BehaviorTree;

	UPROPERTY(EditDefaultsOnly, Category = "Label")
	float LabelViewAmount = 3000.0f;

	UPROPERTY(EditAnywhere, Category = "Patrol")
	class ACPatrolPath* PatrolPath;//Ŭ���� �ۿ����� ������ �� �־�� �Ѵ�. ���ΰ� ������� ��ü�� �����Ұ��̶� softObjectPtr ���

	UPROPERTY(VisibleDefaultsOnly)
	class UWidgetComponent* LabelWidget;

	UPROPERTY(VisibleDefaultsOnly)
	class UCAIBehaviorComponent* Behavior;
};
