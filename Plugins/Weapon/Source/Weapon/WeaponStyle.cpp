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

//void FWeaponStyle::Initialize()
//{
//	if (!StyleSet.IsValid())
//	{
//		StyleSet = Create();
//		FSlateStyleRegistry::RegisterSlateStyle(*StyleSet);
//	}
//}

TSharedRef<FWeaponStyle> FWeaponStyle::Get()
{
	if (Instance == nullptr)
		Instance = MakeShareable(new FWeaponStyle());

	return Instance.ToSharedRef();
}

void FWeaponStyle::Shutdown()
{
	if (Instance.IsValid())
		Instance.Reset();

	//FSlateStyleRegistry::UnRegisterSlateStyle(*StyleSet);
	//ensure(StyleSet.IsUnique());
	//StyleSet.Reset();
}

//TSharedRef< FSlateStyleSet > FWeaponStyle::Create()
//{
//	TSharedRef< FSlateStyleSet > Style = MakeShareable(new FSlateStyleSet("WeaponStyle"));
//	Style->SetContentRoot(IPluginManager::Get().FindPlugin("Weapon")->GetBaseDir() / TEXT("Resources"));
//
//	Style->Set("Weapon.PluginAction", new IMAGE_BRUSH_SVG(TEXT("PlaceholderButtonIcon"), Icon20x20));
//	return Style;
//}

FWeaponStyle::FWeaponStyle()
{
	if (!StyleSet.IsValid())
	{
		TSharedRef< FSlateStyleSet > Style = MakeShareable(new FSlateStyleSet("WeaponStyle"));
		Style->SetContentRoot(IPluginManager::Get().FindPlugin("Weapon")->GetBaseDir() / TEXT("Resources"));

		Style->Set("Weapon.PluginAction", new IMAGE_BRUSH_SVG(TEXT("PlaceholderButtonIcon"), FVector2D(20.0f, 20.0f)));

		FSlateStyleRegistry::RegisterSlateStyle(*Style);
	}
}

FWeaponStyle::~FWeaponStyle()
{
	if (StyleSet.IsValid() == false) return;

	FSlateStyleRegistry::UnRegisterSlateStyle(StyleSetName);
	StyleSet.Reset();
}


void FWeaponStyle::RegisterIcon(const FString& InName, const FString& InPath, const FVector2D& InIconSize, FSlateIcon& OutSlateIcon)
{
	FSlateImageBrush* brush = new FSlateImageBrush(InPath, InIconSize);

	FString name = StyleSetName.ToString() + "." + InName;
	StyleSet->Set(FName(name), brush);

	OutSlateIcon = FSlateIcon(FName(StyleSetName), FName(name));
}