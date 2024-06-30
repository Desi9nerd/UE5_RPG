#pragma once
#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "Weapons/CWeaponStructures.h"
#include "CWeaponAsset.generated.h"

class ACAttachment;
class UCEquipment;
class UCDoAction;
class UCSubAction;

UCLASS()
class RPG_API UCWeaponAsset : public UDataAsset
{
	GENERATED_BODY()

public:
	UCWeaponAsset();
	void BeginPlay(class ACharacter* InOwner, class UCWeaponData** OutWeaponData);//�Ű����� class UCWeaponData** OutWeaponData�� ���. ��ü�� �����ؼ� �����Ѵ�. ���� �����̱� ������ ���� �����͸� ����Ͽ���.

#if WITH_EDITOR //Editor �������� ����
	void PostEditChangeChainProperty(struct FPropertyChangedChainEvent& PropertyChangedEvent) override;
#endif

private:
	UPROPERTY(EditAnywhere)
	TSubclassOf<ACAttachment> AttachmentClass;

	UPROPERTY(EditAnywhere)
	FEquipmentData EquipmentData;

	UPROPERTY(EditAnywhere)
	TSubclassOf<UCEquipment> EquipmentClass;

	UPROPERTY(EditAnywhere)
	TSubclassOf<UCDoAction> DoActionClass;

	UPROPERTY(EditAnywhere)
	TArray<FDoActionData> DoActionDatas; //CWeaopnStructure���� FDoActionData

	UPROPERTY(EditAnywhere)
	TArray<FHitData> HitDatas; //CWeaopnStructure���� FHitData
	
	UPROPERTY(EditAnywhere)
	TSubclassOf<UCSubAction> SubActionClass;

	//********************************************************
	//** ���� �޺�
	UPROPERTY(EditAnywhere)
	TArray<FDoActionData> DoActionDatas_AirborneATK; //CWeaopnStructure���� FDoActionData

	UPROPERTY(EditAnywhere)
	TArray<FHitData> HitDatas_AirborneATK; //CWeaopnStructure���� FHitData
	
	UPROPERTY(EditAnywhere)
	TArray<FDoActionData> DoActionDatas_AirCombo; //CWeaopnStructure���� FDoActionData

	UPROPERTY(EditAnywhere)
	TArray<FHitData> HitDatas_AirCombo; //CWeaopnStructure���� FHitData
	//********************************************************
};