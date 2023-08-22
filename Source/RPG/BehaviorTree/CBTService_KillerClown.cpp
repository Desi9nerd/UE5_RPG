#include "BehaviorTree/CBTService_KillerClown.h"
#include "Global.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "GameFramework/Character.h"
#include "Characters/CAIController.h"
#include "Characters/CEnemy_AI.h"
#include "Components/CStateComponent.h"
#include "Components/CAIBehaviorComponent.h"

#include "GameFramework/Actor.h"

UCBTService_KillerClown::UCBTService_KillerClown()
{
	NodeName = "KillerClown";

	Interval = 0.1f;//ȣ�� ����. 0.1�� ���� ȣ��
	RandomDeviation = 0.0f;
}

void UCBTService_KillerClown::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
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
		

	float distance = ai->GetDistanceTo(target);
	OwnerComp.GetBlackboardComponent()->SetValueAsFloat(UKismetSystemLibrary::MakeLiteralName(FName("BossDistance")), distance);
	//CLog::Log(distance);
		
	if (distance < ActionRange)//���ݹ��� ���� ������
	{
		aiState->SetActionMode();//����

		return;
	}

	if (ActionRange < distance && distance < 2000.0f)
	{
		aiState->SetApproachMode();//���ݹ����� �ƴ϶�� Approach

		return;
	}

	aiState->SetWaitMode();
}
