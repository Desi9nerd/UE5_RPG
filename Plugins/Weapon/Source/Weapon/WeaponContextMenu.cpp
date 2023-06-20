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
	return UCWeaponAsset::StaticClass();//factory가 동작할 클래스로 리턴.
}

FColor FWeaponContextMenu::GetTypeColor() const
{
	return FColor::Blue;
}

uint32 FWeaponContextMenu::GetCategories()
{
	return Category;//만든 것이 소속될 (우클릭해서 나오는)카테고리를 리턴.
}

void FWeaponContextMenu::OpenAssetEditor(const TArray<UObject*>& InObjects,
	TSharedPtr<IToolkitHost> EditWithinLevelEditor)
{
	/*for (UObject* obj : InObjects)
	{
		if (!!obj)
			GLog->Log(obj->GetName());
	}*/
	//위에서 타입을 "DataAsset"으로만 적용해서 DataAsset만 작동한다.

	if (InObjects.Num() < 1)//1 미만으로 선택되었다면(=선택된게 없다면)
		return;

	FWeaponAssetEditor::OpenWindow(InObjects[0]->GetName());//선택된것에 이름 설정해준다. 하나만 선택되었다는 전제(더블클릭해서 열기 때문). 
}