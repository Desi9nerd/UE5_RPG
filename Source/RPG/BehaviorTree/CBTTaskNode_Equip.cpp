#include "BehaviorTree/CBTTaskNode_Equip.h"
#include "Global.h"
#include "Characters/CEnemy_AI.h"
#include "Characters/CAIController.h"
#include "Components/CStateComponent.h"
#include "Weapons/CEquipment.h"

UCBTTaskNode_Equip::UCBTTaskNode_Equip()
{
	NodeName = "Equip";

	bNotifyTick = true;
}

EBTNodeResult::Type UCBTTaskNode_Equip::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::ExecuteTask(OwnerComp, NodeMemory);

	ACAIController* controller = Cast<ACAIController>(OwnerComp.GetOwner());
	ACEnemy_AI* ai = Cast<ACEnemy_AI>(controller->GetPawn());

	UCWeaponComponent* weapon = CHelpers::GetComponent<UCWeaponComponent>(ai);
	CheckNullResult(weapon, EBTNodeResult::Failed);//weapon�� ���ٸ� EBTNodeResult::Failed ����

	if (Type == weapon->GetWeaponType())//������ ���� == ���� ������ ����
		return EBTNodeResult::Succeeded;//���� ������ų �ʿ� ������ �ٷ� Succeeded�� ����

	switch (Type)
	{
		case EWeaponType::Around: weapon->SetAroundMode(); break;
		case EWeaponType::Fist: weapon->SetFistMode(); break;
		case EWeaponType::Sword: weapon->SetSwordMode(); break;//Sword ����
		case EWeaponType::Hammer: weapon->SetHammerMode(); break;//Sword ����
		case EWeaponType::Bow: weapon->SetBowMode(); break;//Bow ����
		case EWeaponType::Warp: weapon->SetWarpMode(); break;//Warp ����
		case EWeaponType::Max: weapon->SetUnarmedMode(); break;
	}

	return EBTNodeResult::InProgress;//���� ������ ���� �� �ֵ��� InProgress ����
}

void UCBTTaskNode_Equip::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickTask(OwnerComp, NodeMemory, DeltaSeconds);

	ACAIController* controller = Cast<ACAIController>(OwnerComp.GetOwner());
	ACEnemy_AI* ai = Cast<ACEnemy_AI>(controller->GetPawn());

	UCWeaponComponent* weapon = CHelpers::GetComponent<UCWeaponComponent>(ai);//weapon�� �����´�.

	const bool* bEquipped = weapon->GetEquipment()->GetEquipped();

	UCStateComponent* state = CHelpers::GetComponent<UCStateComponent>(ai);
	if (state->IsIdleMode() && *bEquipped)
	{
		FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);//Succeeded ����. FinishLatentTask�� ������ ���� Tick ���� ������ ����Ѵ�.

		return;
	}
}

EBTNodeResult::Type UCBTTaskNode_Equip::AbortTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::AbortTask(OwnerComp, NodeMemory);

	ACAIController* controller = Cast<ACAIController>(OwnerComp.GetOwner());
	ACEnemy_AI* ai = Cast<ACEnemy_AI>(controller->GetPawn());

	UCWeaponComponent* weapon = CHelpers::GetComponent<UCWeaponComponent>(ai);
	if (weapon == nullptr)//���Ⱑ ���ٸ�
		return EBTNodeResult::Failed;//���� ����.
	

	bool bBeginEquip = weapon->GetEquipment()->GetBeginEquip();//GetBeginEquip()���� CEquipment�� bBeginEquip�� �־��ش�. bBeginEquip=true�� Equip�� ���۵Ǿ��ٴ� �ǹ̴�.
	if (bBeginEquip == false)
		weapon->GetEquipment()->Begin_Equip();

	weapon->GetEquipment()->End_Equip();

	return EBTNodeResult::Aborted;//Aborted�� ��� ����, ��(Root)�� �ö�. Succeeded�� ��������.
}
