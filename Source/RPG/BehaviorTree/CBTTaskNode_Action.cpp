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

	//Weapon �ȿ� ������ �� ��������(CanMove)�� �ִ�.
	//Player����: Weapon���� DoAction�� �Ѵ�. DoAction�� WeaponStructure�� ������ �ִ�. WeaponSturcture ���� movement�� ���ؼ� ���� ���θ� �����Ѵ�.
	//������ Enemy�� �� �� ����. �׷��� controller->StopMovement()�� ������Ų��.
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
		FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);//Succeeded�� ������. FinishLatentTask�� ������ ���� Tick ���� ������ ����Ѵ�.

		return;
	}
}

EBTNodeResult::Type UCBTTaskNode_Action::AbortTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::AbortTask(OwnerComp, NodeMemory);

	ACAIController* controller = Cast<ACAIController>(OwnerComp.GetOwner());
	ACEnemy_AI* ai = Cast<ACEnemy_AI>(controller->GetPawn());

	UCWeaponComponent* weapon = CHelpers::GetComponent<UCWeaponComponent>(ai);
	if (weapon == nullptr)//���Ⱑ ���ٸ�
		return EBTNodeResult::Failed;//���� �����Ѵ�.

	bool bBeginAction = weapon->GetDoAction()->GetBeginAction();//CDoAction.h���� ���� FORCEINLINE Get�Լ�. bBeginAction ������ true, false ���θ� �˷��ش�.
	//bBeginAction=true�� ������ ���۵Ǿ��ٴ� �ǹ�, false�� ������ �������� �ʾҴٴ� �ǹ̴�.
	if (bBeginAction == false)
		weapon->GetDoAction()->Begin_DoAction();//������ �����Ѵ�.

	weapon->GetDoAction()->End_DoAction();//������ ������.

	return EBTNodeResult::Succeeded;//���� �����Ѵ�.
}
