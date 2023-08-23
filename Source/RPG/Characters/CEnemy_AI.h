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
		uint8 TeamID = 2;//TeamID를 0~255번까지 지정가능하다. 255번은 중립이다. ID 같으면 아군이고 ID가 다르면 적이다.

private:
	UPROPERTY(EditDefaultsOnly, Category = "Label")
		float LabelViewAmount = 3000.0f;

private:
	UPROPERTY(EditAnywhere, Category = "Patrol")
		class ACPatrolPath* PatrolPath;//클래스 밖에서도 지정할 수 있어야 한다. 서로간 만들어진 객체를 참조할것이라서 softObjectPtr 사용

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
	void Hitted() override;//CEnemy의 Hitted 재정의

public:
	void End_Hitted() override;//CEnemy의 End_Hitted 재정의

};
