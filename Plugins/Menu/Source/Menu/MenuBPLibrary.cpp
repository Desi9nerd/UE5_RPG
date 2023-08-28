#include "MenuBPLibrary.h"
#include "Menu.h"
#include "GenericPlatform/GenericApplication.h"
#include "Engine.h"
#include "Widgets/SWindow.h"

UMenuBPLibrary::UMenuBPLibrary(const FObjectInitializer& ObjectInitializer)
: Super(ObjectInitializer)
{

}

int UMenuBPLibrary::GetDisplayCount()
{
	FDisplayMetrics Displays;
	FDisplayMetrics::RebuildDisplayMetrics(Displays);

	return Displays.MonitorInfo.Num();
}
