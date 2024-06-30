#include "Weapons/CWeaponAsset.h"
#include "Global.h"
#include "CAttachment.h"
#include "CEquipment.h"
#include "CDoAction.h"
#include "CSubAction.h"
#include "CWeaponData.h"
#include "GameFramework/Character.h"

UCWeaponAsset::UCWeaponAsset()
{
	AttachmentClass = ACAttachment::StaticClass();//�⺻��
	EquipmentClass = UCEquipment::StaticClass();//�⺻��
}

void UCWeaponAsset::BeginPlay(ACharacter* InOwner, class UCWeaponData** OutWeaponData)
{
	//CWeaponData.h�� �������� �̵��Ͽ��� attachment, equipment, doAction, subAction ��ü ���� ����.

	ACAttachment* attachment = nullptr;
	if (IsValid(AttachmentClass))//AttachmentClass�� ���õǾ� �ִٸ�
	{
		FActorSpawnParameters params;
		params.Owner = InOwner;

		attachment = InOwner->GetWorld()->SpawnActor<ACAttachment>(AttachmentClass, params);
	}

	UCEquipment* equipment = nullptr;
	if (IsValid(EquipmentClass))//EquipmentClass�� ���õǾ� �ִٸ�
	{
		equipment = NewObject<UCEquipment>(this, EquipmentClass);
		equipment->BeginPlay(InOwner, EquipmentData);

		if (IsValid(attachment))//Attachment�� �ִٸ�
		{
			equipment->OnEquipmentBeginEquip.AddDynamic(attachment, &ACAttachment::OnBeginEquip);
			equipment->OnEquipmentUnequip.AddDynamic(attachment, &ACAttachment::OnUnequip);
		}
	}

	UCDoAction* doAction = nullptr;
	if (IsValid(DoActionClass))
	{
		doAction = NewObject<UCDoAction>(this, DoActionClass);
		doAction->BeginPlay(attachment, equipment, InOwner, DoActionDatas, HitDatas);
		doAction->BeginPlay(attachment, equipment, InOwner, DoActionDatas, HitDatas, DoActionDatas_AirborneATK, HitDatas_AirborneATK,DoActionDatas_AirCombo, HitDatas_AirCombo);
					

		if (IsValid(attachment))
		{
			attachment->OnAttachmentBeginCollision.AddDynamic(doAction, &UCDoAction::OnAttachmentBeginCollision);
			attachment->OnAttachmentEndCollision.AddDynamic(doAction, &UCDoAction::OnAttachmentEndCollision);

			attachment->OnAttachmentBeginOverlap.AddDynamic(doAction, &UCDoAction::OnAttachmentBeginOverlap);
			attachment->OnAttachmentEndOverlap.AddDynamic(doAction, &UCDoAction::OnAttachmentEndOverlap);
		}

		if (IsValid(equipment))//Bow_String �۾�
		{
			equipment->OnEquipmentBeginEquip.AddDynamic(doAction, &UCDoAction::OnBeginEquip);
			equipment->OnEquipmentUnequip.AddDynamic(doAction, &UCDoAction::OnUnequip);
		}

		//���� ���� AirborneATK
		if (IsValid(attachment))
		{
			attachment->OnAttachmentBeginCollision.AddDynamic(doAction, &UCDoAction::OnAttachmentBeginCollision);
			attachment->OnAttachmentEndCollision.AddDynamic(doAction, &UCDoAction::OnAttachmentEndCollision);

			attachment->OnAttachmentBeginOverlap.AddDynamic(doAction, &UCDoAction::OnAttachmentBeginOverlap);
			attachment->OnAttachmentEndOverlap.AddDynamic(doAction, &UCDoAction::OnAttachmentEndOverlap);
		}

		//�����޺� AirCombo
		if (IsValid(attachment))
		{
			attachment->OnAttachmentBeginCollision.AddDynamic(doAction, &UCDoAction::OnAttachmentBeginCollision);
			attachment->OnAttachmentEndCollision.AddDynamic(doAction, &UCDoAction::OnAttachmentEndCollision);

			attachment->OnAttachmentBeginOverlap.AddDynamic(doAction, &UCDoAction::OnAttachmentBeginOverlap);
			attachment->OnAttachmentEndOverlap.AddDynamic(doAction, &UCDoAction::OnAttachmentEndOverlap);
		}
	}

	UCSubAction* subAction = nullptr;
	if (IsValid(SubActionClass))
	{
		subAction = NewObject<UCSubAction>(this, SubActionClass);
		subAction->BeginPlay(InOwner, attachment, doAction);
	}
	

	//�Ű����� class UCWeaponData** OutWeaponData�� ����Ͽ���. ��ü�� �����ؼ� �����Ѵ�. ���� �����̱� ������ ���� �����͸� ����Ͽ���.
	*OutWeaponData = NewObject<UCWeaponData>();//�����Ҵ�
	(*OutWeaponData)->Attachment = attachment;
	(*OutWeaponData)->Equipment = equipment;
	(*OutWeaponData)->DoAction = doAction;
	(*OutWeaponData)->SubAction = subAction;
}

#if WITH_EDITOR //Editor �������� ����
void UCWeaponAsset::PostEditChangeChainProperty(FPropertyChangedChainEvent& PropertyChangedEvent)
{
	Super::PostEditChangeChainProperty(PropertyChangedEvent);
	CheckTrue(FApp::IsGame());//������ �������̸� �����ϸ� �� �Ǳ� ������ üũ

	bool bRefresh = false;
	bRefresh |= PropertyChangedEvent.GetPropertyName().Compare(TEXT("DoActionDatas")) == 0;
	bRefresh |= PropertyChangedEvent.GetPropertyName().Compare(TEXT("HitDatas")) == 0;
	bRefresh |= PropertyChangedEvent.GetPropertyName().Compare(TEXT("DoActionDatas_AirborneATK")) == 0;
	bRefresh |= PropertyChangedEvent.GetPropertyName().Compare(TEXT("HitDatas_AirborneATK")) == 0;
	bRefresh |= PropertyChangedEvent.GetPropertyName().Compare(TEXT("DoActionDatas_AirCombo")) == 0;
	bRefresh |= PropertyChangedEvent.GetPropertyName().Compare(TEXT("HitDatas_AirCombo")) == 0;//�����Ϸ��� ������ == 0 �̸� �̸��� �����ϴٴ� �ǹ�.

	if (bRefresh)
	{
		bool bCheck = false;
		bCheck |= PropertyChangedEvent.ChangeType == EPropertyChangeType::ArrayAdd;
		bCheck |= PropertyChangedEvent.ChangeType == EPropertyChangeType::ArrayRemove;
		bCheck |= PropertyChangedEvent.ChangeType == EPropertyChangeType::ArrayClear;
		bCheck |= PropertyChangedEvent.ChangeType == EPropertyChangeType::Duplicate;

		if (bCheck)
		{
			FPropertyEditorModule& prop = FModuleManager::LoadModuleChecked<FPropertyEditorModule>(TEXT("PropertyEditor"));//WITH_EDITOR�� Editor ������ �����ϱ� ������ ��� ����.
			TSharedPtr<IDetailsView> detailsView = prop.FindDetailView(TEXT("WeaponAssetEditorDetailsView"));//WeaponAssetEditor.cpp���� ������ arg.ViewIdentifier�̸� WeaponAssetEditorDetailsView ���. WeaponAssetEditorDetailsView�� ã�´�.

			if (detailsView.IsValid())//detailsView â�� �׷����ٸ�
			{
				detailsView->ForceRefresh();//���ΰ�ħ
			}
		}
	}
}
#endif