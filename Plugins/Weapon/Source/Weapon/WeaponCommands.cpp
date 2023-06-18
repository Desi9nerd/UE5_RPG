#include "WeaponCommands.h"

#define LOCTEXT_NAMESPACE "FWeaponModule"

void FWeaponCommands::RegisterCommands()
{
	UI_COMMAND(PluginAction, "Weapon", "Execute Weapon action", EUserInterfaceActionType::Button, FInputChord());
}

#undef LOCTEXT_NAMESPACE
