#include "Notifies/CAnimNotify_BowString.h"
#include "Global.h"
#include "Components/CWeaponComponent.h"
#include "Weapons/DoActions/CDoAction_Bow.h"

FString UCAnimNotify_BowString::GetNotifyName_Implementation() const
{
	return "End_BowString";
}

void UCAnimNotify_BowString::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	Super::Notify(MeshComp, Animation);
	CheckNull(MeshComp);
	CheckNull(MeshComp->GetOwner());

	UCWeaponComponent* weapon = CHelpers::GetComponent<UCWeaponComponent>(MeshComp->GetOwner());
	CheckNull(weapon);
	CheckNull(weapon->GetDoAction());

	UCDoAction_Bow* bow = Cast<UCDoAction_Bow>(weapon->GetDoAction());
	CheckNull(bow);

	bow->End_BowString();//CDoAction_Bow�� End_BowString�� ���Ͽ� Ȱ ������ �ְ� ����� �տ� ���δ�.
}
