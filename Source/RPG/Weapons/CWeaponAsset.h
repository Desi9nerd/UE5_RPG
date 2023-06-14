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

public:
	FORCEINLINE class ACAttachment* GetAttachment() { return Attachment; }//�ܺο� ������ ���� ��������. 
	FORCEINLINE class UCEquipment* GetEquipment() { return Equipment; }//�ܺο� ������ ���� ��������.
	FORCEINLINE class UCDoAction* GetDoAction() { return DoAction; }//�ܺο� ������ ���� ��������.

public:
	UCWeaponAsset();

	void BeginPlay(class ACharacter* InOwner);

private:
	//UPROPERTY�� �ٿ� ������ �ݷ��Ͱ� �����ϱ� ������ ���� �ְ� �����.
	//UWeaponAsset�� UObject�κ��� ��ӹ޾� Actor�� �����ֱ⿡ ������ ���� �ʾ� ������ �ݷ��Ϳ� ������ �޴´�.
	UPROPERTY() 
		class ACAttachment* Attachment;

	UPROPERTY()
		class UCEquipment* Equipment;

	UPROPERTY()
		class UCDoAction* DoAction;
};
