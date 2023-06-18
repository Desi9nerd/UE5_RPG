#pragma once

#include "CoreMinimal.h"
#include "Framework/Commands/Commands.h"
#include "WeaponStyle.h"

class FWeaponCommands : public TCommands<FWeaponCommands>
{
public:

	FWeaponCommands()
		: TCommands<FWeaponCommands>(TEXT("Weapon"), NSLOCTEXT("Contexts", "Weapon", "Weapon Plugin"), NAME_None, FWeaponStyle::StyleSetName)
	{
	}

	// TCommands<> interface
	virtual void RegisterCommands() override;

public:
	TSharedPtr< FUICommandInfo > PluginAction;

};
