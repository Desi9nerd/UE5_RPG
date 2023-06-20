#include "SWeaponCheckBoxes.h"
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

void SWeaponCheckBoxes::OnCheckStateChanged(ECheckBoxState InState, int32 InIndex)
{
	GLog->Log(FString::FromInt(InIndex));
	GLog->Log(StaticEnum<ECheckBoxState>()->GetValueAsString(InState));
}