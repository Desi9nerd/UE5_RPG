#pragma once
#include "CoreMinimal.h"
#include "Factories/Factory.h"
#include "WeaponAssetFactory.generated.h"

UCLASS()
class WEAPON_API UWeaponAssetFactory : public UFactory
{
	GENERATED_BODY()

public:
	UWeaponAssetFactory();

	//FactoryCreateFile ������ �о�鿩 ������ ���� ������ �� ����Ѵ�.
	//FactorCreateNew�� ���� ������ ������ �� ����Ѵ�.
	UObject* FactoryCreateNew(UClass* InClass, UObject* InParent, FName InName, EObjectFlags Flags, UObject* Context, FFeedbackContext* Warn, FName CallingContext) override;
};