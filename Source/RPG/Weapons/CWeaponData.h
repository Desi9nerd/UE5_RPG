#pragma once
#include "CoreMinimal.h"
#include "CWeaponData.generated.h"

class ACAttachment;
class UCEquipment;
class UCDoAction;
class UCSubAction;

/*  팩토리 패턴 
 *  private으로 변수들을 숨기고 아래의 FORCENILINE 함수들로 변수를 콜한다.
 *  CWeaponData는 상속관계에서 분리되어 있기 때문에 아래의 변수들을 사용하게 될 클래스인 UCWeaponAsset을 friend 클래스로 선언하여 CWeaponAsset 클래스에서 CWeaponData 클래스를 접근할 수 있게 해준다.
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
	//UPROPERTY를 붙여 가비지 콜렉터가 제거하기 전까지 물고 있게 만든다.
	//UWeaponAsset은 UObject로부터 상속받아 Actor의 생성주기에 영향을 받지 않아 가비지 콜렉터에 영향을 받는다.

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
