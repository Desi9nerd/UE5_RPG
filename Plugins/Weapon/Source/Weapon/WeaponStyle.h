#pragma once
#include "CoreMinimal.h"

class WEAPON_API FWeaponStyle
{
public:
	static TSharedRef<FWeaponStyle> Get();
	static void Shutdown();

private:
	static TSharedPtr<FWeaponStyle> Instance;

public:
	FWeaponStyle();
	~FWeaponStyle();

private:
	void RegisterIcon(const FString& InName, const FString& InPath, const FVector2D& InIconSize, FSlateIcon& OutSlateIcon);

private:
	static const FName StyleSetName;

private:
	TSharedPtr<class FSlateStyleSet> StyleSet;

public:
	FSlateIcon ToolBar_Icon;

public:
	const FVector2D DesiredWidth = FVector2D(250, 1000);
};