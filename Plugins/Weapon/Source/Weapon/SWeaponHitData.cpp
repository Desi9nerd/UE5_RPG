#include "SWeaponHitData.h"
#include "WeaponStyle.h"
#include "IDetailPropertyRow.h"
#include "IDetailChildrenBuilder.h"
#include "SWeaponCheckBoxes.h"
#include "DetailWidgetRow.h"

TArray<TSharedPtr<SWeaponCheckBoxes>> SWeaponHitData::CheckBoxes;

TSharedRef<IPropertyTypeCustomization> SWeaponHitData::MakeInstance()
{
	//�ڽ��� Ŭ���� Ÿ���� ���� return���ش�.
	return MakeShareable(new SWeaponHitData());
}

TSharedPtr<SWeaponCheckBoxes> SWeaponHitData::AddCheckBoxes()
{
	TSharedPtr<SWeaponCheckBoxes> checkBoxes = MakeShareable(new SWeaponCheckBoxes());
	int32 index = CheckBoxes.Add(checkBoxes);

	return CheckBoxes[index];
}

void SWeaponHitData::EmptyCheckBoxes()
{
	for (TSharedPtr<SWeaponCheckBoxes> ptr : CheckBoxes)
	{
		if (ptr.IsValid())
			ptr.Reset();
	}

	CheckBoxes.Empty();
}

void SWeaponHitData::CustomizeHeader(TSharedRef<IPropertyHandle> InPropertyHandle, FDetailWidgetRow& InHeaderRow,
	IPropertyTypeCustomizationUtils& InCustomizationUtils)
{
	if (SWeaponCheckBoxes::CanDraw(InPropertyHandle, CheckBoxes.Num()) == false)
	{
		InHeaderRow
			.NameContent()
			[
				InPropertyHandle->CreatePropertyNameWidget()
			]
		.ValueContent()
			.MinDesiredWidth(FWeaponStyle::Get()->DesiredWidth.X)
			.MaxDesiredWidth(FWeaponStyle::Get()->DesiredWidth.Y)
			[
				InPropertyHandle->CreatePropertyValueWidget()
			];

		return;//CanDraw�� false�� �׸��� �ʰ� ����.
	}

	int32 index = InPropertyHandle->GetIndexInArray();//GetIndexInArray()�� Array �ȿ����� ��ȣ�� ����.
	CheckBoxes[index]->SetUtilities(InCustomizationUtils.GetPropertyUtilities());////Header,Children,Header,Children.������ �ݵȴ�.
	
	FString name = InPropertyHandle->GetPropertyDisplayName().ToString();//0,1,2..ǥ���ϴ� name
	name = "Hit Data - " + name;
	
	InHeaderRow
		.NameContent()
		[
			SNew(SBorder)//SBorder�� �� ���� ����� �� ���.
			.BorderImage(FWeaponStyle::Get()->Array_Image.Get())
		[
			InPropertyHandle->CreatePropertyNameWidget(FText::FromString(name))
		]
		]
	.ValueContent()
		.MinDesiredWidth(FWeaponStyle::Get()->DesiredWidth.X)
		.MaxDesiredWidth(FWeaponStyle::Get()->DesiredWidth.Y)
		[
			CheckBoxes[index]->Draw(true)//CheckBoxes�� �׷��ش�(=�����Ѵ�).bBackground�� true���� �־�//�־� alpha����0.1f�� ��� �̹����� ������ �ش� ���� ��ó�� ���̰� ������ش�.
		];
}

void SWeaponHitData::CustomizeChildren(TSharedRef<IPropertyHandle> InPropertyHandle,
	IDetailChildrenBuilder& InChildBuilder, IPropertyTypeCustomizationUtils& InCustomizationUtils)
{
	if (SWeaponCheckBoxes::CanDraw(InPropertyHandle, CheckBoxes.Num()) == false)
	{
		uint32 number = 0;
		InPropertyHandle->GetNumChildren(number);

		for (uint32 i = 0; i < number; i++)
		{
			TSharedPtr<IPropertyHandle> handle = InPropertyHandle->GetChildHandle(i);
			IDetailPropertyRow& row = InChildBuilder.AddProperty(handle.ToSharedRef());

			TSharedPtr<SWidget> name;
			TSharedPtr<SWidget> value;

			if(i == 0)
			{
				TSharedRef<IPropertyHandle> temp = handle.ToSharedRef();

				uint32 numtemp = 0;
				temp->GetNumChildren(numtemp);		//numtemp : ����ü �迭 ������

				for(uint32 j = 0; j < numtemp; j++)
				{
					TSharedPtr<IPropertyHandle> tempHandle = temp->GetChildHandle(j);	//�迭�� �����ϳ��� ������ ��������

					uint32 test = 0;
					tempHandle->GetNumChildren(test);
					IDetailPropertyRow& tempRow = InChildBuilder.AddProperty(tempHandle.ToSharedRef());

					TSharedPtr<SWidget> tempname;
					TSharedPtr<SWidget> tempvalue;

					tempRow.GetDefaultWidgets(tempname, tempvalue);

					tempRow.CustomWidget()
						.NameContent()
						[
							tempname.ToSharedRef()
						]
					.ValueContent()
						.MinDesiredWidth(FWeaponStyle::Get()->DesiredWidth.X)
						.MaxDesiredWidth(FWeaponStyle::Get()->DesiredWidth.Y)
						[
							tempvalue.ToSharedRef()
						];
				}
			}
			else
			{
				row.GetDefaultWidgets(name, value);

				row.CustomWidget()
					.NameContent()
					[
						name.ToSharedRef()
					]
				.ValueContent()
					.MinDesiredWidth(FWeaponStyle::Get()->DesiredWidth.X)
					.MaxDesiredWidth(FWeaponStyle::Get()->DesiredWidth.Y)
					[
						value.ToSharedRef()
					];
			}
		}

		return;//CanDraw�� false�� �׸��� �ʰ� ����.
	}

	int32 index = InPropertyHandle->GetIndexInArray();//GetIndexInArray()�� Array �ȿ����� ��ȣ�� ����.
	CheckBoxes[index]->DrawProperties(InPropertyHandle, &InChildBuilder);
}