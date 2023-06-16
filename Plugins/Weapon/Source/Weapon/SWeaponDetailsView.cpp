#include "SWeaponDetailsView.h"
#include "DetailLayoutBuilder.h"
#include "DetailCategoryBuilder.h"
#include "IDetailPropertyRow.h"
#include "Weapons/CWeaponAsset.h"

TSharedRef<IDetailCustomization> SWeaponDetailsView::MakeInstance()
{
	//자신의 클래스 타입을 만들어서 return해준다.
	return MakeShareable(new SWeaponDetailsView());
}

void SWeaponDetailsView::CustomizeDetails(IDetailLayoutBuilder& DetailBuilder)
{
	UClass* type = UCWeaponAsset::StaticClass();//UClass 타입 하나를 받아온다.

	DetailBuilder.HideCategory("CWeaponAsset");//CWeaponAsset 카테고리를 숨겨준다. 현재 작업의 편의를 위해 숨겨주고 추후에 보여주게 만들 예정이다. 

	//Class Settings
	{
		//.EditCategory 해당 타입에 해당 카테고리가 있으면 그 카테고리를 return, 없으면 새로 만들어서 return
		//ClassSettings는 없으므로 새로 만들어서 return
		IDetailCategoryBuilder& category = DetailBuilder.EditCategory("ClassSettings", FText::FromString("Class Settings"));
		//CWeaponAsset에서 직렬화된 변수들을 카테고리에 추가한다.
		category.AddProperty("AttachmentClass", type);//CWeaponAsset의 AttachmentClass를 카테고리에 추가.
		category.AddProperty("EquipmentClass", type);//CWeaponAsset의 EquipmentClass를 카테고리에 추가.
		category.AddProperty("DoActionClass", type);//CWeaponAsset의 DoActionClass를 카테고리에 추가.
	}

	//EquipmentData
	{
		//.EditCategory 해당 타입에 해당 카테고리가 있으면 그 카테고리를 return, 없으면 새로 만들어서 return
		IDetailCategoryBuilder& category = DetailBuilder.EditCategory("EquipmentData", FText::FromString("Equipment Data"));
		IDetailPropertyRow& row = category.AddProperty("EquipmentData", type);
	}
}