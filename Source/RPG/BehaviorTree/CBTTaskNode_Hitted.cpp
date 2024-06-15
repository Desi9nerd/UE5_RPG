#include "BehaviorTree/CBTTaskNode_Hitted.h"
#include "Global.h"
#include "Characters/CEnemy_AI.h"
#include "Characters/CAIController.h"
#include "Components/CStateComponent.h"

UCBTTaskNode_Hitted::UCBTTaskNode_Hitted()
{
	bNotifyTick = true;

	NodeName = "Hitted";
}

EBTNodeResult::Type UCBTTaskNode_Hitted::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::ExecuteTask(OwnerComp, NodeMemory);

	ACAIController* controller = Cast<ACAIController>(OwnerComp.GetOwner());//Enemy의 controller 캐스팅하여 가져온다.

	controller->StopMovement();//맞은 순간 Enemy를 멈추게 해준다.

	return EBTNodeResult::InProgress;//맞으면서 동작 플레이가 되므로 InProgress
}

void UCBTTaskNode_Hitted::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickTask(OwnerComp, NodeMemory, DeltaSeconds);

	ACAIController* controller = Cast<ACAIController>(OwnerComp.GetOwner());
	ACEnemy_AI* ai = Cast<ACEnemy_AI>(controller->GetPawn());

	UCStateComponent* state = CHelpers::GetComponent<UCStateComponent>(ai);
	if (false == state->IsHittedMode())//Hitted가 끝나면
	{
		//Task를 Succeeded로 만들어주고 리턴한다.
		FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
		return;
	}
}
