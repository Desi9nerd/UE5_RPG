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

	//FactoryCreateFile 파일을 읽어들여 에셋을 직접 생성할 때 사용한다.
	//FactorCreateNew는 새로 에셋을 생성할 때 사용한다.
	UObject* FactoryCreateNew(UClass* InClass, UObject* InParent, FName InName, EObjectFlags Flags, UObject* Context, FFeedbackContext* Warn, FName CallingContext) override;
};