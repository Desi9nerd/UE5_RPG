#include "SWeaponEquipmentData.h"
#include "IPropertyUtilities.h"
#include "IDetailPropertyRow.h"
#include "IDetailChildrenBuilder.h"
#include "SWeaponCheckBoxes.h"
#include "DetailWidgetRow.h"

TSharedPtr<SWeaponCheckBoxes> SWeaponEquipmentData::CheckBoxes;//�ʱ�ȭ

TSharedRef<IPropertyTypeCustomization> SWeaponEquipmentData::MakeInstance()
{
	//�ڽ��� Ŭ���� Ÿ���� ���� return���ش�.
	return MakeShareable(new SWeaponEquipmentData());
}

TSharedPtr<SWeaponCheckBoxes> SWeaponEquipmentData::CreateCheckBoxes()
{
	if (CheckBoxes.IsValid())
	{
		CheckBoxes.Reset();
		CheckBoxes = nullptr;
	}

	return CheckBoxes = MakeShareable(new SWeaponCheckBoxes());
}

void SWeaponEquipmentData::CustomizeHeader(TSharedRef<IPropertyHandle> InPropertyHandle, FDetailWidgetRow& InHeaderRow, IPropertyTypeCustomizationUtils& InCustomizationUtils)
{
	CheckBoxes->SetUtilities(InCustomizationUtils.GetPropertyUtilities());

	InHeaderRow
		.NameContent()
		[
			InPropertyHandle->CreatePropertyNameWidget()
		]
	.ValueContent()
		.MinDesiredWidth(FEditorStyle::GetFloat("StandardDialog.MinDesiredSlotWidth"))
		.MaxDesiredWidth(FEditorStyle::GetFloat("StandardDialog.MaxDesiredSlotWidth"))
		[
			CheckBoxes->Draw()//CheckBoxes�� �׷��ش�(=�����Ѵ�).
		];
}

void SWeaponEquipmentData::CustomizeChildren(TSharedRef<IPropertyHandle> InPropertyHandle, IDetailChildrenBuilder& InChildBuilder, IPropertyTypeCustomizationUtils& InCustomizationUtils)
{
	//uint32 number = 0;
	//InPropertyHandle->GetNumChildren(number);

	//for(uint32 i = 0; i < number; i++)
	//{
	//	TSharedPtr<IPropertyHandle> handle = InPropertyHandle->GetChildHandle(i);
	//	IDetailPropertyRow& row = InChildBuilder.AddProperty(handle.ToSharedRef());//handle�� ������ �⺻����� �߰��Ͽ� ������ش�. 
	//	
	//	row.CustomWidget()
	//	.NameContent()
	//	[
	//		handle->CreatePropertyNameWidget()
	//	]
	//	//���̰ų� �÷��� �� Min ���Ϸδ� ����. Max �̻����δ� ����.
	//	.ValueContent()
	//	.MinDesiredWidth(FEditorStyle::GetFloat("StandardDialog.MinDesiredSlotWidth"))
	//	.MaxDesiredWidth(FEditorStyle::GetFloat("StandardDialog.MaxDesiredSlotWidth"))
	//	[
	//		handle->CreatePropertyValueWidget()
	//	];
	//}

	CheckBoxes->DrawProperties(InPropertyHandle, &InChildBuilder);
}