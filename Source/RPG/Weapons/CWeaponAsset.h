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

public:
	FORCEINLINE class ACAttachment* GetAttachment() { return Attachment; }//외부에 생성된 것을 리턴해줌. 
	FORCEINLINE class UCEquipment* GetEquipment() { return Equipment; }//외부에 생성된 것을 리턴해줌.
	FORCEINLINE class UCDoAction* GetDoAction() { return DoAction; }//외부에 생성된 것을 리턴해줌.

public:
	UCWeaponAsset();

	void BeginPlay(class ACharacter* InOwner);

private:
	//UPROPERTY를 붙여 가비지 콜렉터가 제거하기 전까지 물고 있게 만든다.
	//UWeaponAsset은 UObject로부터 상속받아 Actor의 생성주기에 영향을 받지 않아 가비지 콜렉터에 영향을 받는다.
	UPROPERTY() 
		class ACAttachment* Attachment;

	UPROPERTY()
		class UCEquipment* Equipment;

	UPROPERTY()
		class UCDoAction* DoAction;
};
