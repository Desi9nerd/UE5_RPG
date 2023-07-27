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
		TArray<FDoActionData> DoActionDatas; //CWeaopnStructure���� FDoActionData

	UPROPERTY(EditAnywhere)
		TArray<FHitData> HitDatas; //CWeaopnStructure���� FHitData

	UPROPERTY(EditAnywhere)
		TSubclassOf<class UCSubAction> SubActionClass;

public:
	UCWeaponAsset();

	void BeginPlay(class ACharacter* InOwner, class UCWeaponData** OutWeaponData);//�Ű����� class UCWeaponData** OutWeaponData�� ���. ��ü�� �����ؼ� �����Ѵ�. ���� �����̱� ������ ���� �����͸� ����Ͽ���.
	
#if WITH_EDITOR //Editor �������� ����
	void PostEditChangeChainProperty(struct FPropertyChangedChainEvent& PropertyChangedEvent) override;
#endif
};