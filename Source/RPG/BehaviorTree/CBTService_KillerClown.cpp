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

	Interval = 0.1f;//호출 간격. 0.1초 마다 호출
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
		
	if (distance < ActionRange)//공격범위 내로 들어오면
	{
		aiState->SetActionMode();//공격

		return;
	}

	if (ActionRange < distance && distance < 2000.0f)
	{
		aiState->SetApproachMode();//공격범위가 아니라면 Approach

		return;
	}

	aiState->SetWaitMode();
}
