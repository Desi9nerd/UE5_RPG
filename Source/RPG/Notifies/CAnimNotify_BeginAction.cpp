#include "CAnimNotify_BeginAction.h"
#include "Global.h"
#include "Components/CWeaponComponent.h"
#include "Weapons/CDoAction.h"

FString UCAnimNotify_BeginAction::GetNotifyName_Implementation() const
{
	return "Begin_DoAction";//Notify�̸��� Begin_DoAction���� ����.
}

void UCAnimNotify_BeginAction::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
	const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation, EventReference);

	CheckNull(MeshComp);//MeshComp �ִ��� üũ
	CheckNull(MeshComp->GetOwner());//MeshComp->GetOwner() �ִ��� üũ

	UCWeaponComponent* weapon = CHelpers::GetComponent<UCWeaponComponent>(MeshComp->GetOwner());
	CheckNull(weapon);//weapon�� �ִ��� üũ
	CheckNull(weapon->GetDoAction());//weapon ���� GetDoAction()�Լ��� �ִ��� üũ

	if (weapon->GetDoAction()->GetInAction())
		weapon->GetDoAction()->Begin_DoAction();//Begin_DoAction ����
	else if (weapon->GetDoAction()->GetInAction_AirCombo())
		weapon->GetDoAction()->Begin_DoAction_AirCombo();//Begin_DoAction_AirCombo ����
}