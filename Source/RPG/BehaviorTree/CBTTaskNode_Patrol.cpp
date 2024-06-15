#include "BehaviorTree/CBTTaskNode_Patrol.h"
#include "Global.h"
#include "CPatrolPath.h"
#include "Components/SplineComponent.h"
#include "Components/CAIBehaviorComponent.h"
#include "Characters/CEnemy_AI.h"
#include "Characters/CAIController.h"
#include "NavigationSystem.h"

UCBTTaskNode_Patrol::UCBTTaskNode_Patrol()
{
	NodeName = "Patrol";

	bNotifyTick = true;//�ۼ��ؾ� Tick�� ����ȴ�. Tick�� �����Ϸ��� �ݵ�� �ۼ��ؾ� �Ѵ�.
}

EBTNodeResult::Type UCBTTaskNode_Patrol::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::ExecuteTask(OwnerComp, NodeMemory);

	ACAIController* controller = Cast<ACAIController>(OwnerComp.GetOwner());
	ACEnemy_AI* ai = Cast<ACEnemy_AI>(controller->GetPawn());
	UCAIBehaviorComponent* behavior = CHelpers::GetComponent<UCAIBehaviorComponent>(ai);

	//PatrolPath�� ������
	if (IsValid(ai->GetPatrolPath()))
	{
		//������ ��ġ�� Blackboard�� �ִ´�.
		FVector moveToPoint = ai->GetPatrolPath()->GetMoveTo();
		behavior->SetPatrolLocation(moveToPoint);

		if (bDebugMode)//���� ��� DrawDebug
		{
			DrawDebugSphere(ai->GetWorld(), moveToPoint, 25, 25, FColor::Green, true, 5);
		}

		return EBTNodeResult::InProgress;
	}

	//PatrolPath�� ������
	FVector location = ai->GetActorLocation();

	UNavigationSystemV1* navSystem = FNavigationSystem::GetCurrent<UNavigationSystemV1>(ai->GetWorld());
	CheckNullResult(navSystem, EBTNodeResult::Failed);//navSystem�� ���ٸ� EBTNodeResult::Failed ����

	//������� ���� �޴´�.
	FNavLocation point(location);
	while (true)
	{
		//�� �� �ִ� ��ġ�� ���ö����� ��� ������.
		if (navSystem->GetRandomPointInNavigableRadius(location, RandomRadius, point)) break;
	}

	behavior->SetPatrolLocation(point.Location);

	if (bDebugMode)//���� ��� DrawDebug
	{
		DrawDebugSphere(ai->GetWorld(), point.Location, 25, 25, FColor::Green, true, 5);
	}

	return EBTNodeResult::InProgress;
}

void UCBTTaskNode_Patrol::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickTask(OwnerComp, NodeMemory, DeltaSeconds);

	ACAIController* controller = Cast<ACAIController>(OwnerComp.GetOwner());
	ACEnemy_AI* ai = Cast<ACEnemy_AI>(controller->GetPawn());
	UCAIBehaviorComponent* behavior = CHelpers::GetComponent<UCAIBehaviorComponent>(ai);

	FVector location = behavior->GetPatrolLocation();//�̵��� ��ġ�� location ������ ����ش�.
	EPathFollowingRequestResult::Type result = controller->MoveToLocation(location, AcceptanceDistance, false);

	switch (result)
	{
	case EPathFollowingRequestResult::Failed:
	{
		FinishLatentTask(OwnerComp, EBTNodeResult::Failed);
	}
	break;

	case EPathFollowingRequestResult::AlreadyAtGoal:
	{
		if (ai->GetPatrolPath())
			ai->GetPatrolPath()->UpdateIndex();//���� ��ġ�� �������ش�.

		FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
	}
	break;
	}
}

