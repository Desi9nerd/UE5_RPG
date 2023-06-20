#include "WeaponContextMenu.h"
#include "WeaponAssetEditor.h"
#include "Weapons/CWeaponAsset.h"

FWeaponContextMenu::FWeaponContextMenu(EAssetTypeCategories::Type InCategory)
{
	Category = InCategory;
}

FText FWeaponContextMenu::GetName() const
{
	return FText::FromString("DataAsset");
}

UClass* FWeaponContextMenu::GetSupportedClass() const
{
	return UCWeaponAsset::StaticClass();//factory�� ������ Ŭ������ ����.
}

FColor FWeaponContextMenu::GetTypeColor() const
{
	return FColor::Blue;
}

uint32 FWeaponContextMenu::GetCategories()
{
	return Category;//���� ���� �Ҽӵ� (��Ŭ���ؼ� ������)ī�װ��� ����.
}

void FWeaponContextMenu::OpenAssetEditor(const TArray<UObject*>& InObjects,
	TSharedPtr<IToolkitHost> EditWithinLevelEditor)
{
	/*for (UObject* obj : InObjects)
	{
		if (!!obj)
			GLog->Log(obj->GetName());
	}*/
	//������ Ÿ���� "DataAsset"���θ� �����ؼ� DataAsset�� �۵��Ѵ�.

	if (InObjects.Num() < 1)//1 �̸����� ���õǾ��ٸ�(=���õȰ� ���ٸ�)
		return;

	FWeaponAssetEditor::OpenWindow(InObjects[0]->GetName());//���õȰͿ� �̸� �������ش�. �ϳ��� ���õǾ��ٴ� ����(����Ŭ���ؼ� ���� ����). 
}