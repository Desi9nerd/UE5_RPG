#pragma once
#include "CoreMinimal.h"
#include "IPropertyTypeCustomization.h"

class WEAPON_API SWeaponDoActionData
	: public IPropertyTypeCustomization
{
public:
	static TSharedRef<IPropertyTypeCustomization> MakeInstance();
	static TSharedPtr<class SWeaponCheckBoxes> AddCheckBoxes();
	static void EmptyCheckBoxes();//�ѹ��� ����ִ� ������ �Լ�

	void CustomizeHeader(TSharedRef<IPropertyHandle> InPropertyHandle, FDetailWidgetRow& InHeaderRow, IPropertyTypeCustomizationUtils& InCustomizationUtils) override;
	void CustomizeChildren(TSharedRef<IPropertyHandle> InPropertyHandle, IDetailChildrenBuilder& InChildBuilder, IPropertyTypeCustomizationUtils& InCustomizationUtils) override;

private:
	static TArray<TSharedPtr<class SWeaponCheckBoxes>> CheckBoxes;
};