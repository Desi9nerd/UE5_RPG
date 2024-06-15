#include "BehaviorTree/CBTTaskNode_Speed.h"
#include "Global.h"
#include "Characters/CEnemy_AI.h"
#include "Characters/CAIController.h"

UCBTTaskNode_Speed::UCBTTaskNode_Speed()
{
	NodeName = "Speed";
}

EBTNodeResult::Type UCBTTaskNode_Speed::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	//EBTNodeResult::Type�� Composite�� Task �����ؼ� ����Ѵ�.
	Super::ExecuteTask(OwnerComp, NodeMemory);

	ACAIController* controller = Cast<ACAIController>(OwnerComp.GetOwner());//OwnerComp�� GetOwner�κ���(=Enemy_AI) controller�� �����´�.
	ACEnemy_AI* ai = Cast<ACEnemy_AI>(controller->GetPawn());//������ controller�� Pawn�� ����Ͽ� Enemy_AI�� ĳ�����Ѵ�.
	UCMovementComponent* movement = CHelpers::GetComponent<UCMovementComponent>(ai);

	movement->SetSpeed(Type);//CMovementComponent�� SetSpeed�Լ��� Type���� ������ �ӵ��� �ְ� �����Ѵ�.

	return EBTNodeResult::Succeeded;
}
