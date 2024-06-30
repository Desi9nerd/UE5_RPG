#pragma once
#include "CoreMinimal.h"
#include "CWeaponData.generated.h"

class ACAttachment;
class UCEquipment;
class UCDoAction;
class UCSubAction;

/*  ���丮 ���� 
 *  private���� �������� ����� �Ʒ��� FORCENILINE �Լ���� ������ ���Ѵ�.
 *  CWeaponData�� ��Ӱ��迡�� �и��Ǿ� �ֱ� ������ �Ʒ��� �������� ����ϰ� �� Ŭ������ UCWeaponAsset�� friend Ŭ������ �����Ͽ� CWeaponAsset Ŭ�������� CWeaponData Ŭ������ ������ �� �ְ� ���ش�.
 */

UCLASS()
class RPG_API UCWeaponData : public UObject
{
	GENERATED_BODY()
		
private:
	friend class UCWeaponAsset;

public:
	FORCEINLINE ACAttachment* GetAttachment() { return Attachment; }
	FORCEINLINE UCEquipment* GetEquipment() { return Equipment; }
	FORCEINLINE UCDoAction* GetDoAction() { return DoAction; }
	FORCEINLINE UCSubAction* GetSubAction() { return SubAction; }
	FORCEINLINE UCDoAction* GetDoAction_AirCombo() { return DoAction_AirCombo; }

private:
	//UPROPERTY�� �ٿ� ������ �ݷ��Ͱ� �����ϱ� ������ ���� �ְ� �����.
	//UWeaponAsset�� UObject�κ��� ��ӹ޾� Actor�� �����ֱ⿡ ������ ���� �ʾ� ������ �ݷ��Ϳ� ������ �޴´�.

	UPROPERTY()
	ACAttachment* Attachment;

	UPROPERTY()
	UCEquipment* Equipment;

	UPROPERTY()
	UCDoAction* DoAction;

	UPROPERTY()
	UCSubAction* SubAction;
	
	UPROPERTY()
	UCDoAction* DoAction_AirCombo;
};
