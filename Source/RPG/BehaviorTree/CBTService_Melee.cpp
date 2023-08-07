#include "BehaviorTree/CBTService_Melee.h"
#include "Global.h"
#include "GameFramework/Character.h"
#include "Characters/CAIController.h"
#include "Characters/CEnemy_AI.h"
#include "Components/CStateComponent.h"
#include "Components/CAIBehaviorComponent.h"

UCBTService_Melee::UCBTService_Melee()
{
	NodeName = "Melee,Archer";

	Interval = 0.1f;//호출 간격. 0.1초 마다 호출
	RandomDeviation = 0.0f;
}

void UCBTService_Melee::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	ACAIController* controller = Cast<ACAIController>(OwnerComp.GetOwner());
	ACEnemy_AI* ai = Cast<ACEnemy_AI>(controller->GetPawn());
	UCStateComponent* state = CHelpers::GetComponent<UCStateComponent>(ai);
	UCAIBehaviorComponent* aiState = CHelpers::GetComponent<UCAIBehaviorComponent>(ai);

	if (state->IsHittedMode())
	{
		aiState->SetHittedMode();

		return;
	}

	ACharacter* target = aiState->GetTarget();
	if (target == nullptr)//target이 없다면(=시야 범위 내에 적이 없다면)
	{
		aiState->SetPatrolMode();//Patrol

		return;
	}

	float distance = ai->GetDistanceTo(target);
	if (distance < ActionRange)//공격범위 내로 들어오면
	{
		aiState->SetActionMode();//공격

		return;
	}

	aiState->SetApproachMode();//공격범위가 아니라면 Approach
}
