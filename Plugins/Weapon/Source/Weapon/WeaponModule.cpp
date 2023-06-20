#include "WeaponModule.h"
#include "WeaponStyle.h"
#include "WeaponCommand.h"
#include "WeaponContextMenu.h"
#include "IAssetTools.h"
#include "AssetToolsModule.h"
#include "Misc/MessageDialog.h"
#include "ToolMenus.h"

static const FName WeaponTabName("Weapon");

#define LOCTEXT_NAMESPACE "FWeaponModule"
IMPLEMENT_MODULE(FWeaponModule, Weapon)

void FWeaponModule::StartupModule()
{
	IAssetTools& assetTools = FModuleManager::LoadModuleChecked<FAssetToolsModule>("AssetTools").Get();
	EAssetTypeCategories::Type categories = assetTools.RegisterAdvancedAssetCategory("WeaponAsset", FText::FromString("Weapon"));//

	ContextMenu = MakeShareable(new FWeaponContextMenu(categories));
	assetTools.RegisterAssetTypeActions(ContextMenu.ToSharedRef());

	Command = MakeShareable(new FWeaponCommand());
	Command->Startup();
}

void FWeaponModule::ShutdownModule()
{
	if (ContextMenu.IsValid())
		ContextMenu.Reset();

	if (Command.IsValid())
		Command.Reset();

	FWeaponStyle::Shutdown();
}

#undef LOCTEXT_NAMESPACE
	