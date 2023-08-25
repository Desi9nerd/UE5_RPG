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
		TArray<FHitData_ZombieM> HitDatas_ZombieM; //CWeaopnStructure���� FHitData_ZombieM

	UPROPERTY(EditAnywhere)
		TSubclassOf<class UCSubAction> SubActionClass;


	//AirCombo
	UPROPERTY(EditAnywhere)
		TArray<FDoActionData> DoActionDatas_AirborneATK; //CWeaopnStructure���� FDoActionData

	UPROPERTY(EditAnywhere)
		TArray<FHitData> HitDatas_AirborneATK; //CWeaopnStructure���� FHitData
	
	UPROPERTY(EditAnywhere)
		TArray<FDoActionData> DoActionDatas_AirCombo; //CWeaopnStructure���� FDoActionData

	UPROPERTY(EditAnywhere)
		TArray<FHitData> HitDatas_AirCombo; //CWeaopnStructure���� FHitData

public:
	UCWeaponAsset();

	void BeginPlay(class ACharacter* InOwner, class UCWeaponData** OutWeaponData);//�Ű����� class UCWeaponData** OutWeaponData�� ���. ��ü�� �����ؼ� �����Ѵ�. ���� �����̱� ������ ���� �����͸� ����Ͽ���.
	
#if WITH_EDITOR //Editor �������� ����
	void PostEditChangeChainProperty(struct FPropertyChangedChainEvent& PropertyChangedEvent) override;
#endif
};