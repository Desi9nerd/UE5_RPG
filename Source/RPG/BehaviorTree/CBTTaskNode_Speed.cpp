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
	//EBTNodeResult::Type은 Composite과 Task 공유해서 사용한다.
	Super::ExecuteTask(OwnerComp, NodeMemory);

	ACAIController* controller = Cast<ACAIController>(OwnerComp.GetOwner());//OwnerComp의 GetOwner로부터(=Enemy_AI) controller를 가져온다.
	ACEnemy_AI* ai = Cast<ACEnemy_AI>(controller->GetPawn());//가져온 controller의 Pawn을 사용하여 Enemy_AI를 캐스팅한다.
	UCMovementComponent* movement = CHelpers::GetComponent<UCMovementComponent>(ai);

	movement->SetSpeed(Type);//CMovementComponent의 SetSpeed함수로 Type으로 지정한 속도를 넣고 변경한다.

	return EBTNodeResult::Succeeded;
}
