#pragma once

#include "CoreMinimal.h"
#include "Modules/ModuleManager.h"

class FWeaponModule : public IModuleInterface
{
public:
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;

private:
	TSharedPtr<class FWeaponContextMenu> ContextMenu;//언리얼 내 우클릭 대분류목록
};
