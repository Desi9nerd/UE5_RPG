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
    RegisterIcon("ToolBar_Icon", path / "weapon_thumnail_icon.png", FVector2D(40, 40), ToolBar_Icon);

    FSlateStyleRegistry::RegisterSlateStyle(*StyleSet.Get());


    path = FPaths::EngineConfigDir() / "Editor" / "Slate" / "Common/Selection.png";//Engine이 설치된 Config 경로에 있는 해당 png 이미지의 파일 경로
    Array_Image = MakeShareable(new FSlateImageBrush(path, FVector2D(8, 8), FLinearColor(1, 1, 1, 0.1f)));//path 경로의 이미지를 (8, 8)사이즈로 해당 색과 alpha값으로 생성한다. 
}

FWeaponStyle::~FWeaponStyle()
{
    if (Array_Image.IsValid())
        Array_Image.Reset();

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