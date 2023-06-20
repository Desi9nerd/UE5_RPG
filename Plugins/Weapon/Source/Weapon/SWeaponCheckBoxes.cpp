#include "SWeaponCheckBoxes.h"
#include "Widgets/Layout/SUniformGridPanel.h"
#include "IPropertyUtilities.h"
#include "IDetailPropertyRow.h"
#include "IDetailChildrenBuilder.h"
#include "DetailWidgetRow.h"

void SWeaponCheckBoxes::AddProperties(TSharedPtr<IPropertyHandle> InHandle)
{
	uint32 number = 0;
	InHandle->GetNumChildren(number);//�ڵ� ���� �ڽ� property�� �� �� �ִ��� �����´�.

	for (uint32 i = 0; i < number; i++)
		InternalDatas.Add(FInternalData(InHandle->GetChildHandle(i)));

}

TSharedRef<SWidget> SWeaponCheckBoxes::Draw(bool bBackground)
{
	TSharedPtr<SUniformGridPanel> panel;
	SAssignNew(panel, SUniformGridPanel);
	panel->SetMinDesiredSlotWidth(150);//�ּ��� 150���� ����.

	for (int32 i = 0; i < InternalDatas.Num(); i++)
	{
		//�׷��� �ִ� �κ�
		panel->AddSlot(i, 0)//���ٷ� �ִ´�.
			[
				SNew(SCheckBox)
				.IsChecked(InternalDatas[i].bChecked)
			.OnCheckStateChanged(this, &SWeaponCheckBoxes::OnCheckStateChanged, i)//i�� ���� �Ķ����
			[
				SNew(STextBlock)
				.Text(FText::FromString(InternalDatas[i].Name))//InternalDatas�� �̸��� ���
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