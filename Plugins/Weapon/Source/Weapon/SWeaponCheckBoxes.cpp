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
    InHandle->GetNumChildren(number);//핸들 내의 자식 property가 몇 개 있는지 가져온다.

    for (uint32 i = 0; i < number; i++)
        InternalDatas.Add(FInternalData(InHandle->GetChildHandle(i)));
}

TSharedRef<SWidget> SWeaponCheckBoxes::Draw(bool bBackground)
{
    TSharedPtr<SUniformGridPanel> panel;
    SAssignNew(panel, SUniformGridPanel);//SNew와 다르게 변수를 선언해 놓는 SAssignNew
    panel->SetMinDesiredSlotWidth(150);//최소폭 150으로 설정.

    for (int32 i = 0; i < InternalDatas.Num(); i++)
    {
        panel->AddSlot(i, 0)//한줄만 사용, 여러줄사용하려면 ex) i % 5
            [
                SNew(SCheckBox)//체크박스는 컨텐츠 영역을 가지고 있다.
                .IsChecked(InternalDatas[i].bChecked)//체크 여부 출력, 0과 1로만 판단
            .OnCheckStateChanged(this, &SWeaponCheckBoxes::OnCheckStateChanged, i)//체크 했는지 판단
            [
                SNew(STextBlock)
                .Text(FText::FromString(InternalDatas[i].Name))//InternalDatas의 이름들 출력
            ]
            ];
    }

    if (bBackground == false)
        return panel.ToSharedRef();//추가를 하게되면, 추가된 그자체를 return


    TSharedPtr<SBorder> border = SNew(SBorder)//한 줄 생성.
        .BorderImage(FWeaponStyle::Get()->Array_Image.Get())
        [
            panel.ToSharedRef()//panel를 컨텐츠 영역으로 넣어준다.
        ];

    return border.ToSharedRef();//border를 리턴.
}

void SWeaponCheckBoxes::DrawProperties(TSharedRef<IPropertyHandle> InPropertyHandle, IDetailChildrenBuilder* InChildrenBuilder)
{
    for (int32 i = 0; i < InternalDatas.Num(); i++)
    {
        // 체크박스에 체크를 바꾸게 되는지 확인, 바꾸지 안았다면 그릴 필요가 없다.
        if (InternalDatas[i].bChecked == false)//그릴 필요가 없는 경우
            continue;
        // 각 줄에 식별자
        TSharedPtr<IPropertyHandle> handle = InPropertyHandle->GetChildHandle(i);
        // 자식부분을 담당, 이 Handle이 기본모양을 추가해서 만들어준다. 커스텀 마이징도 가능하다
        IDetailPropertyRow& row = InChildrenBuilder->AddProperty(handle.ToSharedRef());

        //초기세팅1//FString name = FString("Name ") + FString::FromInt(i + 1);
        TSharedPtr<SWidget> name;
        TSharedPtr<SWidget> value;

        row.GetDefaultWidgets(name, value);

        row.CustomWidget()
            .NameContent()
            [
                //초기세팅1//handle->CreatePropertyNameWidget()
                name.ToSharedRef()
            ]
        //줄이거나 늘렸을 때 Min 이하로는 고정. Max 이상으로는 고정.
        .ValueContent()
            .MinDesiredWidth(FWeaponStyle::Get()->DesiredWidth.X)
            .MaxDesiredWidth(FWeaponStyle::Get()->DesiredWidth.Y)
            [
                //초기세팅1//handle->CreatePropertyValueWidget()
                value.ToSharedRef()
            ];
    }
}

void SWeaponCheckBoxes::SetUtilities(TSharedPtr<IPropertyUtilities> InUtilities)
{
    //외부 객체 받아오기
    Utilities = InUtilities;
}

void SWeaponCheckBoxes::OnCheckStateChanged(ECheckBoxState InState, int32 InIndex)
{
    //상태를 바뀌는걸 확인했기때문에, 값을 뒤집어 줘서 다시 안그려지게 만들어준다.
    InternalDatas[InIndex].bChecked = !InternalDatas[InIndex].bChecked;

    SWeaponDetailsView::OnRefreshByCheckBoxes();
    {
        // ForceRefresh이 콜이되면 새로고침이 되면서 다시 그려진다.
        Utilities->ForceRefresh();
    }
    SWeaponDetailsView::OffRefreshByCheckBoxes();

}

bool SWeaponCheckBoxes::CanDraw(TSharedPtr<IPropertyHandle> InHandle, int InCount)
{
    bool bCheck = true;
    bCheck &= InCount > 0;//배열의 개수 InCount가 0보다 큰지

    int32 index = InHandle->GetIndexInArray();//InHandle에서 배열 인덱스번호를 가져온다.
    bCheck &= index >= 0;
    bCheck &= index < InCount;//인덱스가 InCount 배열의 개수보다 작은지//크면 범위를 벗어나서 그릴 수 없다.

    return bCheck;
}

void SWeaponCheckBoxes::CheckDefaultObject(int32 InIndex, UObject* InValue)
{
    UObject* val = nullptr;//기본값
    InternalDatas[InIndex].Handle->GetValue(val);//해당 Property에 선택한 값

    if (!!val && InValue != val)//nullptr이거나 선택한 값이랑 기본값이 다르다면
        InternalDatas[InIndex].bChecked = true;
}

void SWeaponCheckBoxes::CheckDefaultValue(int32 InIndex, float InValue)
{
    float val = 0.0f;//기본값
    InternalDatas[InIndex].Handle->GetValue(val);//해당 Property에 선택한 값

    if (InValue != val)//선택한 값이랑 기본값이 다르다면
        InternalDatas[InIndex].bChecked = true;
}

void SWeaponCheckBoxes::CheckDefaultValue(int32 InIndex, bool InValue)
{
    bool val = false;//기본값
    InternalDatas[InIndex].Handle->GetValue(val);//해당 Property에 선택한 값

    if (InValue != val)//선택한 값이랑 기본값이 다르다면
        InternalDatas[InIndex].bChecked = true;
}

void SWeaponCheckBoxes::CheckDefaultValue(int32 InIndex, const FVector& InValue)
{
    FVector val = FVector::ZeroVector;//기본값
    InternalDatas[InIndex].Handle->GetValue(val);//해당 Property에 선택한 값

    if (InValue != val)//선택한 값이랑 기본값이 다르다면
        InternalDatas[InIndex].bChecked = true;
}