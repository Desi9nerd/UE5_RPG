#include "BehaviorTree/CBTService_Range.h"
#include "Global.h"
#include "GameFramework/Character.h"
#include "Characters/CAIController.h"
#include "Characters/CEnemy_AI.h"
#include "Components/CStateComponent.h"
#include "Components/CAIBehaviorComponent.h"

//활 공격
#include "Components/CWeaponComponent.h"
#include "Weapons/SubActions/CSubAction_Bow.h"

UCBTService_Range::UCBTService_Range()
{
	NodeName = "Range";

	Interval = 0.1f;//호출 간격. 0.1초 마다 호출
	RandomDeviation = 0.0f;
}

void UCBTService_Range::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	ACAIController* controller = Cast<ACAIController>(OwnerComp.GetOwner());
	ACEnemy_AI* ai = Cast<ACEnemy_AI>(controller->GetPawn());
	UCStateComponent* state = CHelpers::GetComponent<UCStateComponent>(ai);
	UCAIBehaviorComponent* aiState = CHelpers::GetComponent<UCAIBehaviorComponent>(ai);

	if (bDrawDebug)
	{
		FVector start = ai->GetActorLocation();//ACEnemy_AI의 위치
		start.Z -= 25;

		FVector end = start;
		end.Z += 50;

		DrawDebugCylinder(ai->GetWorld(), start, end, AvoidRange, 10, FColor::Red, true, Interval, 0, 0);//디버깅 원기둥 그리기
	}

	if (state->IsHittedMode())
	{
		aiState->SetHittedMode();

		return;
	}

	//무기 캐스팅, CSubAction_Bow 캐스팅
	UCWeaponComponent* weapon = CHelpers::GetComponent<UCWeaponComponent>(ai);
	UCSubAction_Bow* bow = nullptr;
	if (!!weapon && weapon->GetWeaponType() == EWeaponType::Bow)
		bow = Cast<UCSubAction_Bow>(weapon->GetSubAction());


	ACharacter* target = aiState->GetTarget();
	if (target == nullptr)//target이 없다면(=시야 범위 내에 적이 없다면)
	{
		//EAIFocusPriorty: Gameplay(Gameplay모드), LastFocusPriority(마지막 Focus 받은애를 우선순위에서 제거), Move(이동 Focus)
		controller->ClearFocus(EAIFocusPriority::Gameplay);//바라보는 Focus를 Gameplay 모드에서 제거
		aiState->SetWaitMode();//타겟이 없다면 Wait모드로 만들어준다.


		if (!!bow)
			bow->Released();


		return;
	}

	//감지가 되었을 때
	controller->SetFocus(target);//감지가 된 target쪽으로 SetFocus하여 바라보게 만든다.

	float distance = ai->GetDistanceTo(target);//현재 나의 위치에서 target까지의 거리를 구한다.
	if (distance < AvoidRange)//거리가 설정한 회피범위 보다 작다면
	{
		aiState->SetAvoidMode();//Avoid 모드로 만들어준다.

		if (!!bow)
			bow->Released();


		return;//회피를 하고 리턴
	}

	
	if (!!bow)
		bow->Pressed();
	
	aiState->SetActionMode();//Action 모드로 만들어준다. BT_Range에 Action 블랙보드 밑에 Equip, Action를 연결하여 무기를 장착하고 공격하게 만든다.
}
