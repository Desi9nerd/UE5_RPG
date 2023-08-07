#include "BehaviorTree/CBTTaskNode_SetFocus.h"
#include "Global.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Characters/CEnemy_AI.h"
#include "Characters/CAIController.h"
#include "Components/CStateComponent.h"
#include "Components/CWeaponComponent.h"
#include "Weapons/CDoAction.h"

UCBTTaskNode_SetFocus::UCBTTaskNode_SetFocus()
{
	NodeName = "SetFocus";

	bNotifyTick = true;
}

EBTNodeResult::Type UCBTTaskNode_SetFocus::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::ExecuteTask(OwnerComp, NodeMemory);

	ACAIController* controller = Cast<ACAIController>(OwnerComp.GetAIOwner());
	//참고: OwnerComp.GetOwner()에서 OwnerComp.GetAIOwner로 수정하였다.
	//ACEnemy_AI* ai = Cast<ACEnemy_AI>(controller->GetPawn());
	ACharacter* Target = Cast<ACharacter>(OwnerComp.GetBlackboardComponent()->GetValueAsObject(FName("Target")));

	if(nullptr == Target)
	{
		return EBTNodeResult::Failed;
	}

	controller->SetFocus(Target, EAIFocusPriority::Gameplay);

	return EBTNodeResult::Succeeded;
}

void UCBTTaskNode_SetFocus::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickTask(OwnerComp, NodeMemory, DeltaSeconds);

	ACAIController* controller = Cast<ACAIController>(OwnerComp.GetAIOwner());
	ACharacter* Target = Cast<ACharacter>(OwnerComp.GetBlackboardComponent()->GetValueAsObject(FName("Target")));

	controller->SetFocus(Target, EAIFocusPriority::Gameplay);
}
