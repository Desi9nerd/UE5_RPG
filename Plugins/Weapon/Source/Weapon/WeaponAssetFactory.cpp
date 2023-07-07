#include "WeaponAssetFactory.h"
#include "Weapons/CWeaponAsset.h"

UWeaponAssetFactory::UWeaponAssetFactory()
{
	bCreateNew = true;//없으면 터질 수 있다.

	//Factory.h의 SupportedClass 사용.
	//Q.어떤 클래스 타입에 대한 factory냐? A.WeaponAsset의 StaticClass다. 
	SupportedClass = UCWeaponAsset::StaticClass();
}

UObject* UWeaponAssetFactory::FactoryCreateNew(UClass* InClass, UObject* InParent, FName InName, EObjectFlags Flags,
	UObject* Context, FFeedbackContext* Warn, FName CallingContext)
{
	//UCWeaponAsset* asset = NewObject<UCWeaponAsset>(InParent, InClass, InName, Flags);
	//
	//return nullptr;

	return NewObject<UCWeaponAsset>(InParent, InClass, InName, Flags);
}