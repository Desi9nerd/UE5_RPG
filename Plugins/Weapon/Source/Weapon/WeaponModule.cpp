#include "WeaponModule.h"
#include "WeaponContextMenu.h"
#include "IAssetTools.h"
#include "AssetToolsModule.h"

#define LOCTEXT_NAMESPACE "FWeaponModule"
IMPLEMENT_MODULE(FWeaponModule, Weapon)

void FWeaponModule::StartupModule()
{
	IAssetTools& assetTools = FModuleManager::LoadModuleChecked<FAssetToolsModule>("AssetTools").Get();
	EAssetTypeCategories::Type categories = assetTools.RegisterAdvancedAssetCategory("WeaponAsset", FText::FromString("Weapon"));//
	ContextMenu = MakeShareable(new FWeaponContextMenu(categories));
	assetTools.RegisterAssetTypeActions(ContextMenu.ToSharedRef());
}

void FWeaponModule::ShutdownModule()
{
	if (ContextMenu.IsValid())
		ContextMenu.Reset();
}

#undef LOCTEXT_NAMESPACE