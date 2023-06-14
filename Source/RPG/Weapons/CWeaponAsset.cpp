#include "CWeaponAsset.h"
#include "Global.h"
#include "CAttachment.h"
#include "CEquipment.h"
#include "CDoAction.h"
#include "GameFramework/Character.h"

UCWeaponAsset::UCWeaponAsset()
{
	AttachmentClass = ACAttachment::StaticClass();//기본값
	EquipmentClass = UCEquipment::StaticClass();//기본값
	DoActionClass = UCDoAction::StaticClass();//기본값
}

void UCWeaponAsset::BeginPlay(ACharacter* InOwner)
{
	if (!!AttachmentClass)//AttachmentClass가 선택되어 있다면
	{
		FActorSpawnParameters params;
		params.Owner = InOwner;

		Attachment = InOwner->GetWorld()->SpawnActor<ACAttachment>(AttachmentClass, params);
	}

	if (!!EquipmentClass)//EquipmentClass가 선택되어 있다면
	{
		Equipment = NewObject<UCEquipment>(this, EquipmentClass);
		Equipment->BeginPlay(InOwner, EquipmentData);

		if (!!Attachment)//Attachment가 있다면
		{
			Equipment->OnEquipmentBeginEquip.AddDynamic(Attachment, &ACAttachment::OnBeginEquip);
			Equipment->OnEquipmentUnequip.AddDynamic(Attachment, &ACAttachment::OnUnequip);
		}
	}
	 
	if(!!DoActionClass)
	{
		DoAction = NewObject<UCDoAction>(this, DoActionClass);
		DoAction->BeginPlay(Attachment, Equipment, InOwner, DoActionDatas, HitDatas);

		if (!!Attachment)
		{
			Attachment->OnAttachmentBeginCollision.AddDynamic(DoAction, &UCDoAction::OnAttachmentBeginCollision);
			Attachment->OnAttachmentEndCollision.AddDynamic(DoAction, &UCDoAction::OnAttachmentEndCollision);

			Attachment->OnAttachmentBeginOverlap.AddDynamic(DoAction, &UCDoAction::OnAttachmentBeginOverlap);
			Attachment->OnAttachmentEndOverlap.AddDynamic(DoAction, &UCDoAction::OnAttachmentEndOverlap);
		}
	}
}

