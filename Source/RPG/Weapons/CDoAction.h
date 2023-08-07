#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Weapons/CWeaponStructures.h"
#include "CDoAction.generated.h"

UCLASS(Abstract)//DoAction �� ��ü�δ� ��üȭ�Ǹ� �� �Ǳ� ������ Abstract�� �ٿ��ش�.
class RPG_API UCDoAction : public UObject
{
	GENERATED_BODY()

public:
	FORCEINLINE bool GetBeginAction() { return bBeginAction; }
	FORCEINLINE bool GetInAction() { return bInAction; }
	FORCEINLINE bool GetInAction_AirCombo() { return bInAction_AirCombo; }

public:
	UCDoAction();

	virtual void BeginPlay //������ �� �� �ֵ��� virtual�� �����.
	(
		class ACAttachment* InAttachment,
		class UCEquipment* InEquipment,
		class ACharacter* InOwner,
		const TArray<FDoActionData>& InDoActionDatas,//CWeaponStucture���� //FDoActionData 
		const TArray<FHitData>& InHitDatas //CWeaponStucture���� FHitData

	);

	virtual void BeginPlay //������ �� �� �ֵ��� virtual�� �����.
	(
		class ACAttachment* InAttachment, 
		class UCEquipment* InEquipment, 
		ACharacter* InOwner, 
		const TArray<FDoActionData>& InDoActionDatas, 
		const TArray<FHitData>& InHitDatas,

		const TArray<FDoActionData>& InAirborneATKDatas,
		const TArray<FHitData>& InAirborneATKHitDatas,

		const TArray<FDoActionData>& InDoAirActionDatas, 
		const TArray<FHitData>& InAirHitDatas
	);

	virtual void Tick(float InDeltaTime) { }

public:
	//������ �� �� �ֵ��� virtual�� �����.
	virtual void DoAction();
	virtual void Begin_DoAction();
	virtual void End_DoAction();

	//�������� ���� ����
	virtual void AirborneInitATK() {};
	//���� �޺�
	virtual void DoAction_AirCombo();
	virtual void Begin_DoAction_AirCombo();
	virtual void End_DoAction_AirCombo();

	//�и�
	virtual void Parrying_Start();
	virtual void Parrying_End();

public:
	//CDoAction_Bow���� ������ ��Ű������ virtual�� �����.
	//BeginEquip�� �� �浹ü�� ���ְ� Unequip�� �� �浹ü�� ���ش�.	
	UFUNCTION()
		virtual void OnBeginEquip() { }

	UFUNCTION()
		virtual void OnUnequip() { }

public:
	UFUNCTION()
		virtual void OnAttachmentBeginCollision() {}

	UFUNCTION()
		virtual void OnAttachmentEndCollision() {}

	UFUNCTION()
		virtual void OnAttachmentBeginOverlap(class ACharacter* InAttacker, AActor* InAttackCauser, class ACharacter* InOther) { }

	UFUNCTION()
		virtual void OnAttachmentEndOverlap(class ACharacter* InAttacker, class ACharacter* InOther) { }
	
protected:
	bool bInAction;//Action ������ üũ�ϴ� ����.DoAction�� true, End_DoAction�� false�� ������ش�.
	bool bBeginAction;

	bool bInAction_AirCombo;//AirCombo ���������� üũ�ϴ� ����. DoAction_AirCombo�� true, End_DoAction_AirCombo�� false�� ������ش�.
	bool bBeginAction_AirCombo;

	bool InitialLaunchATK;

	class ACharacter* OwnerCharacter;
	class UWorld* World;

	class UCMovementComponent* Movement;
	class UCStateComponent* State;

	TArray<FDoActionData> DoActionDatas;
	TArray<FHitData> HitDatas;

	//�����޺� Data
	TArray<FDoActionData> DoActionDatas_AirborneInitATK;
	TArray<FHitData> HitDatas_AirborneInitATK;

	TArray<FDoActionData> DoActionDatas_AirCombo;
	TArray<FHitData> HitDatas_AirCombo;
};
