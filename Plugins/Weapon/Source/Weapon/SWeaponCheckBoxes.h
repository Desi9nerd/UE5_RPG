#pragma once

#include "CoreMinimal.h"

//����Ʈ ������ ���ο��� this�� �����ؾ� �Ǵ� ���, �Ʒ��� ���� �ڱ� �ڽ��� ����ؼ� ��ӹ޴´�. TSharedFromThis<�ڱ� �ڽ�>
class WEAPON_API SWeaponCheckBoxes
	: public TSharedFromThis<SWeaponCheckBoxes>
{
public:
	//IPropertyHandle�� �ϳ��� Property�� ���� �ĺ���
	void AddProperties(TSharedPtr<IPropertyHandle> InHandle);

	TSharedRef<SWidget> Draw(bool bBackground = false);
	void DrawProperties(TSharedRef<IPropertyHandle> InPropertyHandle, IDetailChildrenBuilder* InChildrenBuilder);//�ش� ������Ƽ�� �׸������� ����

	void SetUtilities(TSharedPtr<class IPropertyUtilities> InUtilities);//���ΰ�ħ ���� �ѱ�� ����

private:
	void OnCheckStateChanged(ECheckBoxState InState, int32 InIndex);//CheckBox�� true, false�� �ٲپ��ִ� �Լ�

private:
	//���� ����ü
	struct FInternalData
	{
		bool bChecked;//üũ �Ǿ��°�
		FString Name;//�̸�
		TSharedPtr<IPropertyHandle> Handle;//�ĺ���

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