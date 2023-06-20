#include "SWeaponCheckBoxes.h"
#include "WeaponStyle.h"
#include "SWeaponDetailsView.h"
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
    SAssignNew(panel, SUniformGridPanel);//SNew�� �ٸ��� ������ ������ ���� SAssignNew
    panel->SetMinDesiredSlotWidth(150);//�ּ��� 150���� ����.

    for (int32 i = 0; i < InternalDatas.Num(); i++)
    {
        panel->AddSlot(i, 0)//���ٸ� ���, �����ٻ���Ϸ��� ex) i % 5
            [
                SNew(SCheckBox)//üũ�ڽ��� ������ ������ ������ �ִ�.
                .IsChecked(InternalDatas[i].bChecked)//üũ ���� ���, 0�� 1�θ� �Ǵ�
            .OnCheckStateChanged(this, &SWeaponCheckBoxes::OnCheckStateChanged, i)//üũ �ߴ��� �Ǵ�
            [
                SNew(STextBlock)
                .Text(FText::FromString(InternalDatas[i].Name))//InternalDatas�� �̸��� ���
            ]
            ];
    }

    if (bBackground == false)
        return panel.ToSharedRef();//�߰��� �ϰԵǸ�, �߰��� ����ü�� return


    TSharedPtr<SBorder> border = SNew(SBorder)//�� �� ����.
        .BorderImage(FWeaponStyle::Get()->Array_Image.Get())
        [
            panel.ToSharedRef()//panel�� ������ �������� �־��ش�.
        ];

    return border.ToSharedRef();//border�� ����.
}

void SWeaponCheckBoxes::DrawProperties(TSharedRef<IPropertyHandle> InPropertyHandle, IDetailChildrenBuilder* InChildrenBuilder)
{
    for (int32 i = 0; i < InternalDatas.Num(); i++)
    {
        // üũ�ڽ��� üũ�� �ٲٰ� �Ǵ��� Ȯ��, �ٲ��� �ȾҴٸ� �׸� �ʿ䰡 ����.
        if (InternalDatas[i].bChecked == false)//�׸� �ʿ䰡 ���� ���
            continue;
        // �� �ٿ� �ĺ���
        TSharedPtr<IPropertyHandle> handle = InPropertyHandle->GetChildHandle(i);
        // �ڽĺκ��� ���, �� Handle�� �⺻����� �߰��ؼ� ������ش�. Ŀ���� ����¡�� �����ϴ�
        IDetailPropertyRow& row = InChildrenBuilder->AddProperty(handle.ToSharedRef());

        //�ʱ⼼��1//FString name = FString("Name ") + FString::FromInt(i + 1);
        TSharedPtr<SWidget> name;
        TSharedPtr<SWidget> value;

        row.GetDefaultWidgets(name, value);

        row.CustomWidget()
            .NameContent()
            [
                //�ʱ⼼��1//handle->CreatePropertyNameWidget()
                name.ToSharedRef()
            ]
        //���̰ų� �÷��� �� Min ���Ϸδ� ����. Max �̻����δ� ����.
        .ValueContent()
            .MinDesiredWidth(FWeaponStyle::Get()->DesiredWidth.X)
            .MaxDesiredWidth(FWeaponStyle::Get()->DesiredWidth.Y)
            [
                //�ʱ⼼��1//handle->CreatePropertyValueWidget()
                value.ToSharedRef()
            ];
    }
}

void SWeaponCheckBoxes::SetUtilities(TSharedPtr<IPropertyUtilities> InUtilities)
{
    //�ܺ� ��ü �޾ƿ���
    Utilities = InUtilities;
}

void SWeaponCheckBoxes::OnCheckStateChanged(ECheckBoxState InState, int32 InIndex)
{
    //���¸� �ٲ�°� Ȯ���߱⶧����, ���� ������ �༭ �ٽ� �ȱ׷����� ������ش�.
    InternalDatas[InIndex].bChecked = !InternalDatas[InIndex].bChecked;

    SWeaponDetailsView::OnRefreshByCheckBoxes();
    {
        // ForceRefresh�� ���̵Ǹ� ���ΰ�ħ�� �Ǹ鼭 �ٽ� �׷�����.
        Utilities->ForceRefresh();
    }
    SWeaponDetailsView::OffRefreshByCheckBoxes();

}

bool SWeaponCheckBoxes::CanDraw(TSharedPtr<IPropertyHandle> InHandle, int InCount)
{
    bool bCheck = true;
    bCheck &= InCount > 0;//�迭�� ���� InCount�� 0���� ū��

    int32 index = InHandle->GetIndexInArray();//InHandle���� �迭 �ε�����ȣ�� �����´�.
    bCheck &= index >= 0;
    bCheck &= index < InCount;//�ε����� InCount �迭�� �������� ������//ũ�� ������ ����� �׸� �� ����.

    return bCheck;
}

void SWeaponCheckBoxes::CheckDefaultObject(int32 InIndex, UObject* InValue)
{
    UObject* val = nullptr;//�⺻��
    InternalDatas[InIndex].Handle->GetValue(val);//�ش� Property�� ������ ��

    if (!!val && InValue != val)//nullptr�̰ų� ������ ���̶� �⺻���� �ٸ��ٸ�
        InternalDatas[InIndex].bChecked = true;
}

void SWeaponCheckBoxes::CheckDefaultValue(int32 InIndex, float InValue)
{
    float val = 0.0f;//�⺻��
    InternalDatas[InIndex].Handle->GetValue(val);//�ش� Property�� ������ ��

    if (InValue != val)//������ ���̶� �⺻���� �ٸ��ٸ�
        InternalDatas[InIndex].bChecked = true;
}

void SWeaponCheckBoxes::CheckDefaultValue(int32 InIndex, bool InValue)
{
    bool val = false;//�⺻��
    InternalDatas[InIndex].Handle->GetValue(val);//�ش� Property�� ������ ��

    if (InValue != val)//������ ���̶� �⺻���� �ٸ��ٸ�
        InternalDatas[InIndex].bChecked = true;
}

void SWeaponCheckBoxes::CheckDefaultValue(int32 InIndex, const FVector& InValue)
{
    FVector val = FVector::ZeroVector;//�⺻��
    InternalDatas[InIndex].Handle->GetValue(val);//�ش� Property�� ������ ��

    if (InValue != val)//������ ���̶� �⺻���� �ٸ��ٸ�
        InternalDatas[InIndex].bChecked = true;
}