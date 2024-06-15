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

	ACAIController* controller = Cast<ACAIController>(OwnerComp.GetOwner());//Enemy�� controller ĳ�����Ͽ� �����´�.

	controller->StopMovement();//���� ���� Enemy�� ���߰� ���ش�.

	return EBTNodeResult::InProgress;//�����鼭 ���� �÷��̰� �ǹǷ� InProgress
}

void UCBTTaskNode_Hitted::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickTask(OwnerComp, NodeMemory, DeltaSeconds);

	ACAIController* controller = Cast<ACAIController>(OwnerComp.GetOwner());
	ACEnemy_AI* ai = Cast<ACEnemy_AI>(controller->GetPawn());

	UCStateComponent* state = CHelpers::GetComponent<UCStateComponent>(ai);
	if (false == state->IsHittedMode())//Hitted�� ������
	{
		//Task�� Succeeded�� ������ְ� �����Ѵ�.
		FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
		return;
	}
}
