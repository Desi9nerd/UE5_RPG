#include "CAnimNotify_EndAction.h"
#include "Global.h"
#include "Components/CWeaponComponent.h"
#include "Weapons/CDoAction.h"

FString UCAnimNotify_EndAction::GetNotifyName_Implementation() const
{
	return "End_DoAction";
}

void UCAnimNotify_EndAction::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
	const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation, EventReference);

	CheckNull(MeshComp);
	CheckNull(MeshComp->GetOwner());

	UCWeaponComponent* weapon = CHelpers::GetComponent<UCWeaponComponent>(MeshComp->GetOwner());
	CheckNull(weapon);
	CheckNull(weapon->GetDoAction());

	if (weapon->GetDoAction()->GetInAction())
	{
		weapon->GetDoAction()->End_DoAction();//End_DoAction 실행
	}
	else if (weapon->GetDoAction()->GetInAction_AirCombo())
	{
		weapon->GetDoAction()->End_DoAction_AirCombo();//End_DoAction_AirCombo 실행
	}
}