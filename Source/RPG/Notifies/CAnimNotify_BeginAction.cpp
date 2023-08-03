#include "CAnimNotify_BeginAction.h"
#include "Global.h"
#include "Components/CWeaponComponent.h"
#include "Weapons/CDoAction.h"

FString UCAnimNotify_BeginAction::GetNotifyName_Implementation() const
{
	return "Begin_DoAction";//Notify이름을 Begin_DoAction으로 설정.
}

void UCAnimNotify_BeginAction::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
	const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation, EventReference);

	CheckNull(MeshComp);//MeshComp 있는지 체크
	CheckNull(MeshComp->GetOwner());//MeshComp->GetOwner() 있는지 체크

	UCWeaponComponent* weapon = CHelpers::GetComponent<UCWeaponComponent>(MeshComp->GetOwner());
	CheckNull(weapon);//weapon이 있는지 체크
	CheckNull(weapon->GetDoAction());//weapon 내에 GetDoAction()함수가 있는지 체크

	if (weapon->GetDoAction()->GetInAction())
		weapon->GetDoAction()->Begin_DoAction();//Begin_DoAction 실행
	else if (weapon->GetDoAction()->GetInAction_AirCombo())
		weapon->GetDoAction()->Begin_DoAction_AirCombo();//Begin_DoAction_AirCombo 실행
}