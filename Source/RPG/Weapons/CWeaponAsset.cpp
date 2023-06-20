#include "Weapons/CWeaponAsset.h"
#include "Global.h"
#include "CAttachment.h"
#include "CEquipment.h"
#include "CDoAction.h"
#include "CSubAction.h"
#include "GameFramework/Character.h"

UCWeaponAsset::UCWeaponAsset()
{
	AttachmentClass = ACAttachment::StaticClass();//기본값
	EquipmentClass = UCEquipment::StaticClass();//기본값
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

	if (!!DoActionClass)
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

	if (!!SubActionClass)
	{
		SubAction = NewObject<UCSubAction>(this, SubActionClass);
		SubAction->BeginPlay(InOwner, Attachment, DoAction);
	}
}

#if WITH_EDITOR //Editor 내에서만 수행
void UCWeaponAsset::PostEditChangeChainProperty(FPropertyChangedChainEvent& PropertyChangedEvent)
{
	Super::PostEditChangeChainProperty(PropertyChangedEvent);
	CheckTrue(FApp::IsGame());//게임이 실행중이면 실행하면 안 되기 때문에 체크

	bool bRefresh = false;
	bRefresh |= PropertyChangedEvent.GetPropertyName().Compare("DoActionDatas") == 0;
	bRefresh |= PropertyChangedEvent.GetPropertyName().Compare("HitDatas") == 0;//수정하려는 변수명 == 0 이면 이름이 동일하다는 의미.

	if (bRefresh)
	{
		bool bCheck = false;
		bCheck |= PropertyChangedEvent.ChangeType == EPropertyChangeType::ArrayAdd;
		bCheck |= PropertyChangedEvent.ChangeType == EPropertyChangeType::ArrayRemove;
		bCheck |= PropertyChangedEvent.ChangeType == EPropertyChangeType::ArrayClear;
		bCheck |= PropertyChangedEvent.ChangeType == EPropertyChangeType::Duplicate;

		if (bCheck)
		{
			FPropertyEditorModule& prop = FModuleManager::LoadModuleChecked<FPropertyEditorModule>("PropertyEditor");//WITH_EDITOR로 Editor 내에서 수행하기 때문에 사용 가능.
			TSharedPtr<IDetailsView> detailsView = prop.FindDetailView("WeaponAssetEditorDetailsView");//WeaponAssetEditor.cpp에서 설정한 arg.ViewIdentifier이름 WeaponAssetEditorDetailsView 사용. WeaponAssetEditorDetailsView를 찾는다.

			if (detailsView.IsValid())//detailsView 창이 그려졌다면
				detailsView->ForceRefresh();//새로고침
		}
	}
}
#endif