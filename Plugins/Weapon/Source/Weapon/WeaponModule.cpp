#include "WeaponModule.h"
#include "WeaponStyle.h"
#include "WeaponCommands.h"
#include "Misc/MessageDialog.h"
#include "ToolMenus.h"

static const FName WeaponTabName("Weapon");

#define LOCTEXT_NAMESPACE "FWeaponModule"

void FWeaponModule::StartupModule()
{
	// This code will execute after your module is loaded into memory; the exact timing is specified in the .uplugin file per-module

	FWeaponCommands::Register();
	
	PluginCommands = MakeShareable(new FUICommandList);

	PluginCommands->MapAction(
		FWeaponCommands::Get().PluginAction,
		FExecuteAction::CreateRaw(this, &FWeaponModule::PluginButtonClicked),
		FCanExecuteAction());		
}

void FWeaponModule::ShutdownModule()
{
	// This function may be called during shutdown to clean up your module.  For modules that support dynamic reloading,
	// we call this function before unloading the module.

	UToolMenus::UnRegisterStartupCallback(this);

	UToolMenus::UnregisterOwner(this);

	FWeaponStyle::Shutdown();

	FWeaponCommands::Unregister();
}

void FWeaponModule::PluginButtonClicked()
{
	// Put your "OnButtonClicked" stuff here	
	FMessageDialog::Open(EAppMsgType::Ok, FText::FromString("Test"));
}

#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_MODULE(FWeaponModule, Weapon)