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

	Interval = 0.1f;//ȣ�� ����. 0.1�� ���� ȣ��
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
	if (target == nullptr)//target�� ���ٸ�(=�þ� ���� ���� ���� ���ٸ�)
	{
		aiState->SetPatrolMode();//Patrol

		return;
	}

	float distance = ai->GetDistanceTo(target);
	if (distance < ActionRange)//���ݹ��� ���� ������
	{
		aiState->SetActionMode();//����

		return;
	}

	aiState->SetApproachMode();//���ݹ����� �ƴ϶�� Approach
}
