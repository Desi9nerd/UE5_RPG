#include "SWeaponDetailsView.h"
#include "SWeaponCheckBoxes.h"
#include "SWeaponEquipmentData.h"
#include "DetailLayoutBuilder.h"
#include "DetailCategoryBuilder.h"
#include "IDetailPropertyRow.h"
#include "Weapons/CWeaponAsset.h"

bool SWeaponDetailsView::bRefreshByCheckBoxes = false;//static ���� �ʱ�ȭ.

TSharedRef<IDetailCustomization> SWeaponDetailsView::MakeInstance()
{
	//�ڽ��� Ŭ���� Ÿ���� ���� return���ش�.
	return MakeShareable(new SWeaponDetailsView());
}

void SWeaponDetailsView::CustomizeDetails(IDetailLayoutBuilder& DetailBuilder)
{
	UClass* type = UCWeaponAsset::StaticClass();//UClass Ÿ�� �ϳ��� �޾ƿ´�.

	DetailBuilder.HideCategory("CWeaponAsset");//CWeaponAsset ī�װ��� �����ش�. ���� �۾��� ���Ǹ� ���� �����ְ� ���Ŀ� �����ְ� ���� �����̴�. 

	//Class Settings
	{
		//.EditCategory �ش� Ÿ�Կ� �ش� ī�װ��� ������ �� ī�װ��� return, ������ ���� ���� return
		//ClassSettings�� �����Ƿ� ���� ���� return
		IDetailCategoryBuilder& category = DetailBuilder.EditCategory("ClassSettings", FText::FromString("Class Settings"));
		//CWeaponAsset���� ����ȭ�� �������� ī�װ��� �߰��Ѵ�.
		category.AddProperty("AttachmentClass", type);//CWeaponAsset�� AttachmentClass�� ī�װ��� �߰�.
		category.AddProperty("EquipmentClass", type);//CWeaponAsset�� EquipmentClass�� ī�װ��� �߰�.
		category.AddProperty("DoActionClass", type);//CWeaponAsset�� DoActionClass�� ī�װ��� �߰�.
	}

	//EquipmentData
	{
		//.EditCategory �ش� Ÿ�Կ� �ش� ī�װ��� ������ �� ī�װ��� return, ������ ���� ���� return
		IDetailCategoryBuilder& category = DetailBuilder.EditCategory("EquipmentData", FText::FromString("Equipment Data"));
		IDetailPropertyRow& row = category.AddProperty("EquipmentData", type);

		if (bRefreshByCheckBoxes == false)//���ΰ�ħ�� �ƴ� ��
		{
			TSharedPtr<SWeaponCheckBoxes> checkBoxes = SWeaponEquipmentData::CreateCheckBoxes();//ī�װ��� ó���� ������� �� checkBox�� �����.
			checkBoxes->AddProperties(row.GetPropertyHandle());//checkBoxes�� ������ ���� Handle�� �߰�
		}
	}
}