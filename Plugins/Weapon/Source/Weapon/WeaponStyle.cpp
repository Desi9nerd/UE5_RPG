#include "WeaponStyle.h"
#include "WeaponModule.h"
#include "Framework/Application/SlateApplication.h"
#include "Styling/SlateStyleRegistry.h"
#include "Slate/SlateGameResources.h"
#include "Interfaces/IPluginManager.h"
#include "Styling/SlateStyleMacros.h"

#define RootToContentDir Style->RootToContentDir

const FName FWeaponStyle::StyleSetName = "WeaponStyle";

TSharedPtr<FWeaponStyle> FWeaponStyle::Instance = nullptr;

//const ISlateStyle& FWeaponStyle::Get()
//{
//	return *Instance;
//}

TSharedRef<FWeaponStyle> FWeaponStyle::Get()
{
	if (Instance == nullptr)
		Instance = MakeShareable(new FWeaponStyle());

	return Instance.ToSharedRef();
}

void FWeaponStyle::Shutdown()
{
	//FSlateStyleRegistry::UnRegisterSlateStyle(*Instance);
	//ensure(Instance.IsUnique());
	//Instance.Reset();
}

FWeaponStyle::FWeaponStyle()
{
}

const FVector2D Icon16x16(16.0f, 16.0f);
const FVector2D Icon20x20(20.0f, 20.0f);

TSharedRef< FSlateStyleSet > FWeaponStyle::Create()
{
	TSharedRef< FSlateStyleSet > Style = MakeShareable(new FSlateStyleSet("WeaponStyle"));
	Style->SetContentRoot(IPluginManager::Get().FindPlugin("Weapon")->GetBaseDir() / TEXT("Resources"));

	Style->Set("Weapon.PluginAction", new IMAGE_BRUSH_SVG(TEXT("PlaceholderButtonIcon"), Icon20x20));
	return Style;
}


