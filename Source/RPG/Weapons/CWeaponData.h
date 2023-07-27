#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "CWeaponData.generated.h"

UCLASS()
class RPG_API UCWeaponData : public UObject
{
	GENERATED_BODY()

//���丮�� ����
//private���� �������� ����� �Ʒ��� FORCENILINE �Լ���� ������ ���Ѵ�.
//CWeaponData�� ��Ӱ��迡�� �и��Ǿ� �ֱ� ������ �Ʒ��� �������� ����ϰ� �� Ŭ������ UCWeaponAsset�� friend Ŭ������ �����Ͽ� CWeaponAsset Ŭ�������� CWeaponData Ŭ������ ������ �� �ְ� ���ش�.
private:
	friend class UCWeaponAsset;

public:
	FORCEINLINE class ACAttachment* GetAttachment() { return Attachment; }//�ܺο� ������ ���� ��������. 
	FORCEINLINE class UCEquipment* GetEquipment() { return Equipment; }//�ܺο� ������ ���� ��������.
	FORCEINLINE class UCDoAction* GetDoAction() { return DoAction; }//�ܺο� ������ ���� ��������.
	FORCEINLINE class UCSubAction* GetSubAction() { return SubAction; }//�ܺο� ������ ���� ��������.

private:
	//UPROPERTY�� �ٿ� ������ �ݷ��Ͱ� �����ϱ� ������ ���� �ְ� �����.
	//UWeaponAsset�� UObject�κ��� ��ӹ޾� Actor�� �����ֱ⿡ ������ ���� �ʾ� ������ �ݷ��Ϳ� ������ �޴´�.

	UPROPERTY()
		class ACAttachment* Attachment;

	UPROPERTY()
		class UCEquipment* Equipment;

	UPROPERTY()
		class UCDoAction* DoAction;

	UPROPERTY()
		class UCSubAction* SubAction;
};
