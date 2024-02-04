#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Weapons/CWeaponStructures.h"
#include "CEquipment.generated.h"

//직렬화하여 BP에서 사용가능하도록 DYNAMIC, 여러함수를 받을 수 있도록 MULTICAST 사용.
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FEquipmentEquip);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FEquipmentBeginEquip);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FEquipmentUnequip);

UCLASS()
class RPG_API UCEquipment : public UObject
{
	GENERATED_BODY()

public:
	//실시간으로 bBeginEquip, bEquipped로 가져온다. bEquipped은 주소로 가져온다.
	FORCEINLINE bool GetBeginEquip() { return bBeginEquip; }
	FORCEINLINE const bool* GetEquipped() { return &bEquipped; }
	
	void BeginPlay(class ACharacter* InOwner, const FEquipmentData& InData);

	UFUNCTION(BlueprintNativeEvent)//필요시 BP에서 재정의하기 위해 BlueprintNativeEvent사용.
		void Equip();
		void Equip_Implementation();

	UFUNCTION(BlueprintNativeEvent)
		void Begin_Equip();
		void Begin_Equip_Implementation();

	UFUNCTION(BlueprintNativeEvent)
		void End_Equip();
		void End_Equip_Implementation();

	UFUNCTION(BlueprintNativeEvent)
		void Unequip();
		void Unequip_Implementation();

	FEquipmentEquip OnEquipmentEquip;//DECLARE_DYNAMIC_MULTICAST_DELEGATE 하였다.
	FEquipmentBeginEquip OnEquipmentBeginEquip;//DECLARE_DYNAMIC_MULTICAST_DELEGATE 하였다.
	FEquipmentUnequip OnEquipmentUnequip;//DECLARE_DYNAMIC_MULTICAST_DELEGATE 하였다.

private:
	class ACharacter* OwnerCharacter;
	FEquipmentData Data;
	
	class UCMovementComponent* Movement;
	class UCStateComponent* State;
	
	bool bBeginEquip;//Equip이 시작되었는가 확인
	bool bEquipped;  //Equip이 완료되었는지 확인
};

