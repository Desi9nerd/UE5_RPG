#pragma once
#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "Weapons/CWeaponStructures.h"
#include "CWeaponAsset.generated.h"

UCLASS()
class RPG_API UCWeaponAsset : public UDataAsset
{
	GENERATED_BODY()

private:
	UPROPERTY(EditAnywhere)
		TSubclassOf<class ACAttachment> AttachmentClass;

	UPROPERTY(EditAnywhere)
		FEquipmentData EquipmentData;

	UPROPERTY(EditAnywhere)
		TSubclassOf<class UCEquipment> EquipmentClass;

	UPROPERTY(EditAnywhere)
		TSubclassOf<class UCDoAction> DoActionClass;

	UPROPERTY(EditAnywhere)
		TArray<FDoActionData> DoActionDatas; //CWeaopnStructure내의 FDoActionData

	UPROPERTY(EditAnywhere)
		TArray<FHitData> HitDatas; //CWeaopnStructure내의 FHitData

	UPROPERTY(EditAnywhere)
		TSubclassOf<class UCSubAction> SubActionClass;

public:
	UCWeaponAsset();

	void BeginPlay(class ACharacter* InOwner, class UCWeaponData** OutWeaponData);//매개변수 class UCWeaponData** OutWeaponData를 사용. 객체를 생성해서 리턴한다. 생성 리턴이기 때문에 이차 포인터를 사용하였다.
	
#if WITH_EDITOR //Editor 내에서만 수행
	void PostEditChangeChainProperty(struct FPropertyChangedChainEvent& PropertyChangedEvent) override;
#endif
};