#include "SWeaponCheckBoxes.h"
#include "SWeaponDetailsView.h"
#include "Widgets/Layout/SUniformGridPanel.h"
#include "IPropertyUtilities.h"
#include "IDetailPropertyRow.h"
#include "IDetailChildrenBuilder.h"
#include "DetailWidgetRow.h"

void SWeaponCheckBoxes::AddProperties(TSharedPtr<IPropertyHandle> InHandle)
{
	uint32 number = 0;
	InHandle->GetNumChildren(number);//핸들 내의 자식 property가 몇 개 있는지 가져온다.

	for (uint32 i = 0; i < number; i++)
		InternalDatas.Add(FInternalData(InHandle->GetChildHandle(i)));

}

TSharedRef<SWidget> SWeaponCheckBoxes::Draw(bool bBackground)
{
	TSharedPtr<SUniformGridPanel> panel;
	SAssignNew(panel, SUniformGridPanel);
	panel->SetMinDesiredSlotWidth(150);//최소폭 150으로 설정.

	for (int32 i = 0; i < InternalDatas.Num(); i++)
	{
		//그려서 넣는 부분
		panel->AddSlot(i, 0)//한줄로 넣는다.
			[
				SNew(SCheckBox)
				.IsChecked(InternalDatas[i].bChecked)
			.OnCheckStateChanged(this, &SWeaponCheckBoxes::OnCheckStateChanged, i)//i는 가변 파라미터
			[
				SNew(STextBlock)
				.Text(FText::FromString(InternalDatas[i].Name))//InternalDatas의 이름들 출력
			]
			];
	}

	return panel.ToSharedRef();
}

void SWeaponCheckBoxes::DrawProperties(TSharedRef<IPropertyHandle> InPropertyHandle, IDetailChildrenBuilder* InChildrenBuilder)
{
	for (int32 i = 0; i < InternalDatas.Num(); i++)
	{
		if (InternalDatas[i].bChecked == false)//그릴 필요가 없는 경우
			continue;

		TSharedPtr<IPropertyHandle> handle = InPropertyHandle->GetChildHandle(i);
		IDetailPropertyRow& row = InChildrenBuilder->AddProperty(handle.ToSharedRef());//handle를 가지고 기본모양을 추가하여 만들어준다.

		FString name = FString("Name ") + FString::FromInt(i + 1);

		row.CustomWidget()
		.NameContent()
		[
			handle->CreatePropertyNameWidget()
		]
		//줄이거나 늘렸을 때 Min 이하로는 고정. Max 이상으로는 고정.
		.ValueContent()
		.MinDesiredWidth(FEditorStyle::GetFloat("StandardDialog.MinDesiredSlotWidth"))
		.MaxDesiredWidth(FEditorStyle::GetFloat("StandardDialog.MaxDesiredSlotWidth"))
		[
			handle->CreatePropertyValueWidget()
		];
	}
}

void SWeaponCheckBoxes::SetUtilities(TSharedPtr<IPropertyUtilities> InUtilities)
{
	Utilities = InUtilities;
}

void SWeaponCheckBoxes::OnCheckStateChanged(ECheckBoxState InState, int32 InIndex)
{
	InternalDatas[InIndex].bChecked = !InternalDatas[InIndex].bChecked;//bChecked값을 뒤집어준다.

	SWeaponDetailsView::OnRefreshByCheckBoxes();
	{
		Utilities->ForceRefresh();//새로고침이 된다.
	}
	SWeaponDetailsView::OffRefreshByCheckBoxes();
}