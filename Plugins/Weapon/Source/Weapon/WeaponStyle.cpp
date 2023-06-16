#include "WeaponStyle.h"
#include "Styling/SlateStyle.h"
#include "Styling/SlateStyleRegistry.h"

const FName FWeaponStyle::StyleSetName = "WeaponStyle";

TSharedPtr<FWeaponStyle> FWeaponStyle::Instance = nullptr;

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
}

FWeaponStyle::FWeaponStyle()
{
    StyleSet = MakeShareable(new FSlateStyleSet(StyleSetName));


    FString path = "";

    path = FPaths::ProjectPluginsDir() / "Weapon" / "Resources";
    RegisterIcon("ToolBar_Icon", path / "weapon_thumnail_icon.png", FVector2D(20, 20), ToolBar_Icon);

    FSlateStyleRegistry::RegisterSlateStyle(*StyleSet.Get());
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