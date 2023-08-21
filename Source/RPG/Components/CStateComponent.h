#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Characters/ECharacterTypes.h"//Enum
#include "CStateComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FStateTypeChanged, EStateType, InPrevType, EStateType, InNewType);

UCLASS()
class RPG_API UCStateComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	FORCEINLINE bool IsIdleMode() { return Type == EStateType::Idle; }
	FORCEINLINE bool IsDodgeMode() { return Type == EStateType::Dodge; }
	FORCEINLINE bool IsEquipMode() { return Type == EStateType::Equip; }
	FORCEINLINE bool IsHittedMode() { return Type == EStateType::Hitted; }
	FORCEINLINE bool IsDeadMode() { return Type == EStateType::Dead; }
	FORCEINLINE bool IsActionMode() { return Type == EStateType::Action; }
	FORCEINLINE bool IsParryingMode() { return Type == EStateType::Parrying; }
	FORCEINLINE bool IsSubActionMode() { return bInSubActionMode; }

	FORCEINLINE EStateType GetStateType() { return Type; }

public:
	UCStateComponent();

protected:
	virtual void BeginPlay() override;

public:
	void SetIdleMode();
	void SetDodgeMode();
	void SetEquipMode();
	void SetHittedMode();
	void SetDeadMode();
	void SetActionMode();
	void SetParryingMode();
	void OnSubActionMode();
	void OffSubActionMode();

private:
	void ChangeType(EStateType InType);

public:
	FStateTypeChanged OnStateTypeChanged;

private:
	EStateType Type;

private:
	bool bInSubActionMode;

public:
	//class ACPlayer* OwnerCPlayer;

};