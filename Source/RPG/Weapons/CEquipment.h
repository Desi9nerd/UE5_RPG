#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Weapons/CWeaponStructures.h"
#include "CEquipment.generated.h"

//����ȭ�Ͽ� BP���� ��밡���ϵ��� DYNAMIC, �����Լ��� ���� �� �ֵ��� MULTICAST ���.
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FEquipmentEquip);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FEquipmentBeginEquip);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FEquipmentUnequip);

UCLASS()
class RPG_API UCEquipment : public UObject
{
	GENERATED_BODY()

public:
	//�ǽð����� bBeginEquip, bEquipped�� �����´�. bEquipped�� �ּҷ� �����´�.
	FORCEINLINE bool GetBeginEquip() { return bBeginEquip; }
	FORCEINLINE const bool* GetEquipped() { return &bEquipped; }
	
	void BeginPlay(class ACharacter* InOwner, const FEquipmentData& InData);

	UFUNCTION(BlueprintNativeEvent)//�ʿ�� BP���� �������ϱ� ���� BlueprintNativeEvent���.
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

	FEquipmentEquip OnEquipmentEquip;//DECLARE_DYNAMIC_MULTICAST_DELEGATE �Ͽ���.
	FEquipmentBeginEquip OnEquipmentBeginEquip;//DECLARE_DYNAMIC_MULTICAST_DELEGATE �Ͽ���.
	FEquipmentUnequip OnEquipmentUnequip;//DECLARE_DYNAMIC_MULTICAST_DELEGATE �Ͽ���.

private:
	class ACharacter* OwnerCharacter;
	FEquipmentData Data;
	
	class UCMovementComponent* Movement;
	class UCStateComponent* State;
	
	bool bBeginEquip;//Equip�� ���۵Ǿ��°� Ȯ��
	bool bEquipped;  //Equip�� �Ϸ�Ǿ����� Ȯ��
};

