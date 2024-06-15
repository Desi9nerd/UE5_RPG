#pragma once
#include "CoreMinimal.h"

class FSlateStyleSet;
struct FSlateImageBrush;

class WEAPON_API FWeaponStyle
{
public:
	FWeaponStyle();
	~FWeaponStyle();
	static TSharedRef<FWeaponStyle> Get();
	static void Shutdown();

	FSlateIcon ToolBar_Icon;

	TSharedPtr<FSlateImageBrush> Array_Image;

	const FVector2D DesiredWidth = FVector2D(250, 1000);

private:
	void RegisterIcon(const FString& InName, const FString& InPath, const FVector2D& InIconSize, FSlateIcon& OutSlateIcon);

	static TSharedPtr<FWeaponStyle> Instance;

	static const FName StyleSetName;
	
	TSharedPtr<FSlateStyleSet> StyleSet;
};