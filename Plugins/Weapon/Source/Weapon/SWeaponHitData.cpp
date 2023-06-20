#include "SWeaponHitData.h"
#include "WeaponStyle.h"
#include "IPropertyUtilities.h"
#include "IDetailPropertyRow.h"
#include "IDetailChildrenBuilder.h"
#include "SWeaponCheckBoxes.h"
#include "DetailWidgetRow.h"

TArray<TSharedPtr<SWeaponCheckBoxes>> SWeaponHitData::CheckBoxes;

TSharedRef<IPropertyTypeCustomization> SWeaponHitData::MakeInstance()
{
	//자신의 클래스 타입을 만들어서 return해준다.
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
		return;//CanDraw가 false면 그리지 않고 리턴.

	int32 index = InPropertyHandle->GetIndexInArray();//GetIndexInArray()는 Array 안에서의 번호를 리턴.
	CheckBoxes[index]->SetUtilities(InCustomizationUtils.GetPropertyUtilities());//Header,Children,Header,Children..순서로 콜된다.

	FString name = InPropertyHandle->GetPropertyDisplayName().ToString();//0,1,2..표시하는 name
	name = "Hit Data - " + name;

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

void SWeaponHitData::CustomizeChildren(TSharedRef<IPropertyHandle> InPropertyHandle,
	IDetailChildrenBuilder& InChildBuilder, IPropertyTypeCustomizationUtils& InCustomizationUtils)
{
	if (SWeaponCheckBoxes::CanDraw(InPropertyHandle, CheckBoxes.Num()) == false)
		return;//CanDraw가 false면 그리지 않고 리턴.

	int32 index = InPropertyHandle->GetIndexInArray();//GetIndexInArray()는 Array 안에서의 번호를 리턴.
	CheckBoxes[index]->DrawProperties(InPropertyHandle, &InChildBuilder);
}