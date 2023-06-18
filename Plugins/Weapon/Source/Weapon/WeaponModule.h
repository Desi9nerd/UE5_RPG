#pragma once

#include "CoreMinimal.h"
#include "Modules/ModuleManager.h"

class FToolBarBuilder;
class FMenuBuilder;

class FWeaponModule : public IModuleInterface
{
public:

	/** IModuleInterface implementation */
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;
	
	/** This function will be bound to Command. */
	void PluginButtonClicked();
	

private:
	TSharedPtr<class FUICommandList> PluginCommands;
	TSharedPtr<class FWeaponContextMenu> ContextMenu;//언리얼 내 우클릭 대분류목록
};
