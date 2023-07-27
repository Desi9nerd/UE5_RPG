#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Weapons/CWeaponStructures.h"
#include "CDoAction.generated.h"

UCLASS(Abstract)//DoAction 그 자체로는 객체화되면 안 되기 때문에 Abstract을 붙여준다.
class RPG_API UCDoAction : public UObject
{
	GENERATED_BODY()

public:
	FORCEINLINE bool GetInAction() { return bInAction; }

public:
	UCDoAction();

	virtual void BeginPlay //재정의 할 수 있도록 virtual로 만든다.
	(
		class ACAttachment* InAttachment,
		class UCEquipment* InEquipment,
		class ACharacter* InOwner,
		const TArray<FDoActionData>& InDoActionDatas,//CWeaponStucture내의 FDoActionData 
		const TArray<FHitData>& InHitDatas //CWeaponStucture내의 FHitData
	);

	virtual void Tick(float InDeltaTime) { }

public:
	//재정의 할 수 있도록 virtual로 만든다.
	virtual void DoAction();
	virtual void Begin_DoAction();
	virtual void End_DoAction();

public:
	//CDoAction_Bow에서 재정의 시키기위해 virtual로 만든다.
	//BeginEquip될 때 충돌체를 꺼주고 Unequip될 때 충돌체를 켜준다.	
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
	bool bInAction;//Action 중인지 체크하는 변수.DoAction에 true, End_DoAction에 false로 만들어준다.
	bool bBeginAction;

	class ACharacter* OwnerCharacter;
	class UWorld* World;

	class UCMovementComponent* Movement;
	class UCStateComponent* State;

	TArray<FDoActionData> DoActionDatas;
	TArray<FHitData> HitDatas;
};
