#pragma once

#include "CoreMinimal.h"
#include "Characters/CEnemy.h"
#include "CEnemy_AI.generated.h"

UCLASS()
class RPG_API ACEnemy_AI : public ACEnemy
{
	GENERATED_BODY()

private:
	UPROPERTY(EditDefaultsOnly, Category = "AI")
		class UBehaviorTree* BehaviorTree;

	UPROPERTY(EditAnywhere, Category = "AI")
		uint8 TeamID = 2;//TeamID�� 0~255������ ���������ϴ�. 255���� �߸��̴�. ID ������ �Ʊ��̰� ID�� �ٸ��� ���̴�.

private:
	UPROPERTY(EditDefaultsOnly, Category = "Label")
		float LabelViewAmount = 3000.0f;

private:
	UPROPERTY(EditAnywhere, Category = "Patrol")
		class ACPatrolPath* PatrolPath;//Ŭ���� �ۿ����� ������ �� �־�� �Ѵ�. ���ΰ� ������� ��ü�� �����Ұ��̶� softObjectPtr ���

private:
	UPROPERTY(VisibleDefaultsOnly)
		class UWidgetComponent* LabelWidget;

private:
	UPROPERTY(VisibleDefaultsOnly)
		class UCAIBehaviorComponent* Behavior;

public:
	FORCEINLINE uint8 GetTeamID() { return TeamID; }
	FORCEINLINE class UBehaviorTree* GetBehaviorTree() { return BehaviorTree; }
	FORCEINLINE class ACPatrolPath* GetPatrolPath() { return PatrolPath; }

public:
	ACEnemy_AI();

protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;

private:
	void UpdateLabelRenderScale();

protected:
	void Hitted() override;//CEnemy�� Hitted ������

public:
	void End_Hitted() override;//CEnemy�� End_Hitted ������

};
