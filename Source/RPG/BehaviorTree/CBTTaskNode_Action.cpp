#include "BehaviorTree/CBTTaskNode_Action.h"
#include "Global.h"
#include "Characters/CEnemy_AI.h"
#include "Characters/CAIController.h"
#include "Components/CStateComponent.h"
#include "Components/CWeaponComponent.h"
#include "Weapons/CDoAction.h"
#include "Weapons/DoActions/CDoAction_Combo.h"

UCBTTaskNode_Action::UCBTTaskNode_Action()
{
	NodeName = "Action";

	bNotifyTick = true;
	
}

EBTNodeResult::Type UCBTTaskNode_Action::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::ExecuteTask(OwnerComp, NodeMemory);

	ACAIController* controller = Cast<ACAIController>(OwnerComp.GetOwner());
	ACEnemy_AI* ai = Cast<ACEnemy_AI>(controller->GetPawn());

	UCWeaponComponent* weapon = CHelpers::GetComponent<UCWeaponComponent>(ai);
	CheckNullResult(weapon, EBTNodeResult::Failed);

	//UCDoAction_Combo* combo = Cast<UCDoAction_Combo>(weapon->GetDoAction());
	//if(!!combo)
	//{
	//	CheckTrueResult(combo->GetbEnable(), EBTNodeResult::InProgress);
	//}

	//Weapon 안에 공격할 때 정지여부(CanMove)가 있다.
	//Player기준: Weapon에서 DoAction을 한다. DoAction은 WeaponStructure를 가지고 있다. WeaponSturcture 내의 movement를 통해서 정지 여부를 결정한다.
	//하지만 Enemy는 알 수 없다. 그래서 controller->StopMovement()로 정지시킨다.
	controller->StopMovement();
	weapon->DoAction();

	//CheckFalseResult(weapon->GetDoAction()->GetInAction());

	return EBTNodeResult::InProgress;
}

void UCBTTaskNode_Action::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickTask(OwnerComp, NodeMemory, DeltaSeconds);

	ACAIController* controller = Cast<ACAIController>(OwnerComp.GetOwner());
	ACEnemy_AI* ai = Cast<ACEnemy_AI>(controller->GetPawn());


	UCWeaponComponent* weapon = CHelpers::GetComponent<UCWeaponComponent>(ai);
	UCStateComponent* state = CHelpers::GetComponent<UCStateComponent>(ai);

	UCDoAction_Combo* combo = Cast<UCDoAction_Combo>(weapon->GetDoAction());
	if(!!combo)
	{
		combo->bExist = true;
	}

	bool bCheck = true;
	bCheck &= (state->IsIdleMode());
	bCheck &= (weapon->GetDoAction()->GetInAction() == false);

	if (bCheck)
	{
		FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);//Succeeded로 끝낸다. FinishLatentTask는 리턴이 없는 Tick 같은 곳에서 사용한다.

		return;
	}
}

EBTNodeResult::Type UCBTTaskNode_Action::AbortTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::AbortTask(OwnerComp, NodeMemory);

	ACAIController* controller = Cast<ACAIController>(OwnerComp.GetOwner());
	ACEnemy_AI* ai = Cast<ACEnemy_AI>(controller->GetPawn());

	UCWeaponComponent* weapon = CHelpers::GetComponent<UCWeaponComponent>(ai);
	if (weapon == nullptr)//무기가 없다면
		return EBTNodeResult::Failed;//실패 종료한다.

	bool bBeginAction = weapon->GetDoAction()->GetBeginAction();//CDoAction.h에서 만든 FORCEINLINE Get함수. bBeginAction 변수의 true, false 여부를 알려준다.
	//bBeginAction=true면 공격이 시작되었다는 의미, false면 공격이 시작하지 않았다는 의미다.
	if (bBeginAction == false)
		weapon->GetDoAction()->Begin_DoAction();//공격을 시작한다.

	weapon->GetDoAction()->End_DoAction();//공격을 끝낸다.

	return EBTNodeResult::Succeeded;//성공 종료한다.
}
