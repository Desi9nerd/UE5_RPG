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
	CheckNullResult(weapon, EBTNodeResult::Failed);//weapon이 없다면 EBTNodeResult::Failed 리턴

	if (Type == weapon->GetWeaponType())//장착될 무기 == 현재 장착된 무기
		return EBTNodeResult::Succeeded;//새로 장착시킬 필요 없으니 바로 Succeeded를 리턴

	switch (Type)
	{
		case EWeaponType::Around: weapon->SetAroundMode(); break;
		case EWeaponType::Fist: weapon->SetFistMode(); break;
		case EWeaponType::Sword: weapon->SetSwordMode(); break;//Sword 장착
		case EWeaponType::Hammer: weapon->SetHammerMode(); break;//Sword 장착
		case EWeaponType::Bow: weapon->SetBowMode(); break;//Bow 장착
		case EWeaponType::Warp: weapon->SetWarpMode(); break;//Warp 장착
		case EWeaponType::Max: weapon->SetUnarmedMode(); break;
	}

	return EBTNodeResult::InProgress;//장착 동작이 나올 수 있도록 InProgress 리턴
}

void UCBTTaskNode_Equip::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickTask(OwnerComp, NodeMemory, DeltaSeconds);

	ACAIController* controller = Cast<ACAIController>(OwnerComp.GetOwner());
	ACEnemy_AI* ai = Cast<ACEnemy_AI>(controller->GetPawn());

	UCWeaponComponent* weapon = CHelpers::GetComponent<UCWeaponComponent>(ai);//weapon을 가져온다.

	const bool* bEquipped = weapon->GetEquipment()->GetEquipped();

	UCStateComponent* state = CHelpers::GetComponent<UCStateComponent>(ai);
	if (state->IsIdleMode() && *bEquipped)
	{
		FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);//Succeeded 종료. FinishLatentTask는 리턴이 없는 Tick 같은 곳에서 사용한다.

		return;
	}
}

EBTNodeResult::Type UCBTTaskNode_Equip::AbortTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::AbortTask(OwnerComp, NodeMemory);

	ACAIController* controller = Cast<ACAIController>(OwnerComp.GetOwner());
	ACEnemy_AI* ai = Cast<ACEnemy_AI>(controller->GetPawn());

	UCWeaponComponent* weapon = CHelpers::GetComponent<UCWeaponComponent>(ai);
	if (weapon == nullptr)//무기가 없다면
		return EBTNodeResult::Failed;//실패 종료.
	

	bool bBeginEquip = weapon->GetEquipment()->GetBeginEquip();//GetBeginEquip()으로 CEquipment의 bBeginEquip를 넣어준다. bBeginEquip=true면 Equip이 시작되었다는 의미다.
	if (bBeginEquip == false)
		weapon->GetEquipment()->Begin_Equip();

	weapon->GetEquipment()->End_Equip();

	return EBTNodeResult::Aborted;//Aborted는 취소 마감, 위(Root)로 올라감. Succeeded는 성공종료.
}
