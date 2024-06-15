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

	bNotifyTick = true;//작성해야 Tick이 실행된다. Tick을 실행하려면 반드시 작성해야 한다.
}

EBTNodeResult::Type UCBTTaskNode_Patrol::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::ExecuteTask(OwnerComp, NodeMemory);

	ACAIController* controller = Cast<ACAIController>(OwnerComp.GetOwner());
	ACEnemy_AI* ai = Cast<ACEnemy_AI>(controller->GetPawn());
	UCAIBehaviorComponent* behavior = CHelpers::GetComponent<UCAIBehaviorComponent>(ai);

	//PatrolPath가 있을때
	if (IsValid(ai->GetPatrolPath()))
	{
		//움직일 위치를 Blackboard에 넣는다.
		FVector moveToPoint = ai->GetPatrolPath()->GetMoveTo();
		behavior->SetPatrolLocation(moveToPoint);

		if (bDebugMode)//순찰 경로 DrawDebug
		{
			DrawDebugSphere(ai->GetWorld(), moveToPoint, 25, 25, FColor::Green, true, 5);
		}

		return EBTNodeResult::InProgress;
	}

	//PatrolPath가 없을때
	FVector location = ai->GetActorLocation();

	UNavigationSystemV1* navSystem = FNavigationSystem::GetCurrent<UNavigationSystemV1>(ai->GetWorld());
	CheckNullResult(navSystem, EBTNodeResult::Failed);//navSystem이 없다면 EBTNodeResult::Failed 리턴

	//결과값을 리턴 받는다.
	FNavLocation point(location);
	while (true)
	{
		//갈 수 있는 위치가 나올때까지 계속 돌린다.
		if (navSystem->GetRandomPointInNavigableRadius(location, RandomRadius, point)) break;
	}

	behavior->SetPatrolLocation(point.Location);

	if (bDebugMode)//순찰 경로 DrawDebug
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

	FVector location = behavior->GetPatrolLocation();//이동할 위치를 location 변수에 담아준다.
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
			ai->GetPatrolPath()->UpdateIndex();//다음 위치를 갱신해준다.

		FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
	}
	break;
	}
}

