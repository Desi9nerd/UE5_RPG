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
	AttachmentClass = ACAttachment::StaticClass();//기본값
	EquipmentClass = UCEquipment::StaticClass();//기본값
}

void UCWeaponAsset::BeginPlay(ACharacter* InOwner, class UCWeaponData** OutWeaponData)
{
	//CWeaponData.h로 변수들이 이동하였기 attachment, equipment, doAction, subAction 객체 각각 생성.

	ACAttachment* attachment = nullptr;
	if (IsValid(AttachmentClass))//AttachmentClass가 선택되어 있다면
	{
		FActorSpawnParameters params;
		params.Owner = InOwner;

		attachment = InOwner->GetWorld()->SpawnActor<ACAttachment>(AttachmentClass, params);
	}

	UCEquipment* equipment = nullptr;
	if (IsValid(EquipmentClass))//EquipmentClass가 선택되어 있다면
	{
		equipment = NewObject<UCEquipment>(this, EquipmentClass);
		equipment->BeginPlay(InOwner, EquipmentData);

		if (IsValid(attachment))//Attachment가 있다면
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

		if (IsValid(equipment))//Bow_String 작업
		{
			equipment->OnEquipmentBeginEquip.AddDynamic(doAction, &UCDoAction::OnBeginEquip);
			equipment->OnEquipmentUnequip.AddDynamic(doAction, &UCDoAction::OnUnequip);
		}

		//공중 띄우기 AirborneATK
		if (IsValid(attachment))
		{
			attachment->OnAttachmentBeginCollision.AddDynamic(doAction, &UCDoAction::OnAttachmentBeginCollision);
			attachment->OnAttachmentEndCollision.AddDynamic(doAction, &UCDoAction::OnAttachmentEndCollision);

			attachment->OnAttachmentBeginOverlap.AddDynamic(doAction, &UCDoAction::OnAttachmentBeginOverlap);
			attachment->OnAttachmentEndOverlap.AddDynamic(doAction, &UCDoAction::OnAttachmentEndOverlap);
		}

		//공중콤보 AirCombo
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
	

	//매개변수 class UCWeaponData** OutWeaponData를 사용하였다. 객체를 생성해서 리턴한다. 생성 리턴이기 때문에 이차 포인터를 사용하였다.
	*OutWeaponData = NewObject<UCWeaponData>();//동적할당
	(*OutWeaponData)->Attachment = attachment;
	(*OutWeaponData)->Equipment = equipment;
	(*OutWeaponData)->DoAction = doAction;
	(*OutWeaponData)->SubAction = subAction;
}

#if WITH_EDITOR //Editor 내에서만 수행
void UCWeaponAsset::PostEditChangeChainProperty(FPropertyChangedChainEvent& PropertyChangedEvent)
{
	Super::PostEditChangeChainProperty(PropertyChangedEvent);
	CheckTrue(FApp::IsGame());//게임이 실행중이면 실행하면 안 되기 때문에 체크

	bool bRefresh = false;
	bRefresh |= PropertyChangedEvent.GetPropertyName().Compare(TEXT("DoActionDatas")) == 0;
	bRefresh |= PropertyChangedEvent.GetPropertyName().Compare(TEXT("HitDatas")) == 0;
	bRefresh |= PropertyChangedEvent.GetPropertyName().Compare(TEXT("DoActionDatas_AirborneATK")) == 0;
	bRefresh |= PropertyChangedEvent.GetPropertyName().Compare(TEXT("HitDatas_AirborneATK")) == 0;
	bRefresh |= PropertyChangedEvent.GetPropertyName().Compare(TEXT("DoActionDatas_AirCombo")) == 0;
	bRefresh |= PropertyChangedEvent.GetPropertyName().Compare(TEXT("HitDatas_AirCombo")) == 0;//수정하려는 변수명 == 0 이면 이름이 동일하다는 의미.

	if (bRefresh)
	{
		bool bCheck = false;
		bCheck |= PropertyChangedEvent.ChangeType == EPropertyChangeType::ArrayAdd;
		bCheck |= PropertyChangedEvent.ChangeType == EPropertyChangeType::ArrayRemove;
		bCheck |= PropertyChangedEvent.ChangeType == EPropertyChangeType::ArrayClear;
		bCheck |= PropertyChangedEvent.ChangeType == EPropertyChangeType::Duplicate;

		if (bCheck)
		{
			FPropertyEditorModule& prop = FModuleManager::LoadModuleChecked<FPropertyEditorModule>(TEXT("PropertyEditor"));//WITH_EDITOR로 Editor 내에서 수행하기 때문에 사용 가능.
			TSharedPtr<IDetailsView> detailsView = prop.FindDetailView(TEXT("WeaponAssetEditorDetailsView"));//WeaponAssetEditor.cpp에서 설정한 arg.ViewIdentifier이름 WeaponAssetEditorDetailsView 사용. WeaponAssetEditorDetailsView를 찾는다.

			if (detailsView.IsValid())//detailsView 창이 그려졌다면
			{
				detailsView->ForceRefresh();//새로고침
			}
		}
	}
}
#endif