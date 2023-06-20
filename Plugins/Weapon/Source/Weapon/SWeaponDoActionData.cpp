#include "SWeaponDoActionData.h"
#include "WeaponStyle.h"
#include "IPropertyUtilities.h"
#include "IDetailPropertyRow.h"
#include "IDetailChildrenBuilder.h"
#include "SWeaponCheckBoxes.h"
#include "DetailWidgetRow.h"

TArray<TSharedPtr<SWeaponCheckBoxes>> SWeaponDoActionData::CheckBoxes;

TSharedRef<IPropertyTypeCustomization> SWeaponDoActionData::MakeInstance()
{
	//자신의 클래스 타입을 만들어서 return해준다.
	return MakeShareable(new SWeaponDoActionData());
}

TSharedPtr<SWeaponCheckBoxes> SWeaponDoActionData::AddCheckBoxes()
{
	TSharedPtr<SWeaponCheckBoxes> checkBoxes = MakeShareable(new SWeaponCheckBoxes());
	int32 index = CheckBoxes.Add(checkBoxes);

	return CheckBoxes[index];
}

void SWeaponDoActionData::EmptyCheckBoxes()
{
	for (TSharedPtr<SWeaponCheckBoxes> ptr : CheckBoxes)
	{
		if (ptr.IsValid())
			ptr.Reset();
	}

	CheckBoxes.Empty();
}

void SWeaponDoActionData::CustomizeHeader(TSharedRef<IPropertyHandle> InPropertyHandle, FDetailWidgetRow& InHeaderRow,
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

		return;//CanDraw가 false면 그리지 않고 리턴.
	}

	int32 index = InPropertyHandle->GetIndexInArray();//GetIndexInArray()는 Array 안에서의 번호를 리턴.
	CheckBoxes[index]->SetUtilities(InCustomizationUtils.GetPropertyUtilities());//Header,Children,Header,Children..순서로 콜된다.

	FString name = InPropertyHandle->GetPropertyDisplayName().ToString();//0,1,2..표시하는 name
	name = "DoAction Data - " + name;

	InHeaderRow
		.NameContent()
		[
			SNew(SBorder)//SBorder는 한 줄을 출력할 때 사용.
			.BorderImage(FWeaponStyle::Get()->Array_Image.Get())
		[
			InPropertyHandle->CreatePropertyNameWidget(FText::FromString(name))
		]
		]
	.ValueContent()
		.MinDesiredWidth(FWeaponStyle::Get()->DesiredWidth.X)
		.MaxDesiredWidth(FWeaponStyle::Get()->DesiredWidth.Y)
		[
			CheckBoxes[index]->Draw(true)//CheckBoxes를 그려준다(=생성한다).bBackground에 true값을 넣어주어 alpha값이 0.1f인 흰색 이미지가 겹쳐저 해당 줄이 띠처럼 보이게 만들어준다.
		];
}

void SWeaponDoActionData::CustomizeChildren(TSharedRef<IPropertyHandle> InPropertyHandle,
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
		}//for(i)

		return;//CanDraw가 false면 그리지 않고 리턴.
	}

	int32 index = InPropertyHandle->GetIndexInArray();//GetIndexInArray()는 Array 안에서의 번호를 리턴.
	CheckBoxes[index]->DrawProperties(InPropertyHandle, &InChildBuilder);
}