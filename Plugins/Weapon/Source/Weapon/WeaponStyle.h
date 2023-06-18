#pragma once

#include "CoreMinimal.h"
#include "Styling/SlateStyle.h"

class FWeaponStyle
{
public:
	/** @return The Slate style set for the Shooter game */
	//static const ISlateStyle& Get();
	static TSharedRef<FWeaponStyle> Get();
	static void Shutdown();

private:
	static TSharedPtr<FWeaponStyle> Instance;

public:
	FWeaponStyle();

public:
	static const FName StyleSetName;

private:
	static TSharedRef< class FSlateStyleSet > Create();

};