#pragma once

#include "CoreMinimal.h"

class WEAPON_API SWeaponCheckBoxes
    : public TSharedFromThis<SWeaponCheckBoxes> // ���������� ��ӹ����� �ּҰ� ��ġ�ϰԵȴ�.
{
public:
    //IPropertyHandle�� �ϳ��� Property�� ���� �ĺ���
    void AddProperties(TSharedPtr<IPropertyHandle> InHandle);

    //SNew�� �ֻ��� �θ� SWidget�� �ȴ�. ���۷����� ����ϱ� ������ �����ϰ� ��������
    TSharedRef<SWidget> Draw(bool bBackground = false);//����Ʈ �Ķ���ͷ� �⺻���� false�� ����.

    //�ش� Properties�� �׸��� ���� ����
    void DrawProperties(TSharedRef<IPropertyHandle> InPropertyHandle, IDetailChildrenBuilder* InChildrenBuilder);

    //��ü�� �ܺο��� �ޱ�����,
    void SetUtilities(TSharedPtr<class IPropertyUtilities> InUtilities);

private:
    //üũ�� �Ϸ�Ǿ����� 
    void OnCheckStateChanged(ECheckBoxState InState, int32 InIndex);

public:
    static bool CanDraw(TSharedPtr<IPropertyHandle> InHandle, int InCount);//����ڰ� üũ�� InHandle�� ���ͼ� �迭�� ������ ��ġ�ϴ��� �Ǵ�. �迭 ������ ����� �׸��� �ʵ��� false�� ����.

public:
    //InIndex���� Property ��ȣ�� InValue���� ��.
    void CheckDefaultObject(int32 InIndex, UObject* InValue);
    void CheckDefaultValue(int32 InIndex, float InValue);
    void CheckDefaultValue(int32 InIndex, bool InValue);
    void CheckDefaultValue(int32 InIndex, const FVector& InValue);
    void CheckDefaultClass(int32 InIndex, const TSubclassOf<ACharacter> InClass);

private:
    //���� ����ü ����, ������ ���� ���
    struct FInternalData
    {
        bool bChecked;//üũ �Ǿ��°�
        FString Name;//�̸�
        TSharedPtr<IPropertyHandle> Handle;

        FInternalData(TSharedPtr<IPropertyHandle> InHandle)
        {
            bChecked = false;//�⺻���� false�� ����.
            Handle = InHandle;

            Name = Handle->GetPropertyDisplayName().ToString();//�ڵ� ���� DisplayName�� ����̸����� ����.
        }
    };
    TArray<FInternalData> InternalDatas;//����ü Data ���θ� ������ �迭����

    TSharedPtr<class IPropertyUtilities> Utilities;//��� ����
};