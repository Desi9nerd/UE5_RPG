#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "CStateComponent.generated.h"

UENUM()
enum class EStateType : uint8
{
	Idle = 0, BackStep, Equip, Hitted, Dead, Action, Max,
};
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FStateTypeChanged, EStateType, InPrevType, EStateType, InNewType);

UCLASS()
class RPG_API UCStateComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	FORCEINLINE bool IsIdleMode() { return Type == EStateType::Idle; }
	FORCEINLINE bool IsBackstepMode() { return Type == EStateType::BackStep; }
	FORCEINLINE bool IsEquipMode() { return Type == EStateType::Equip; }
	FORCEINLINE bool IsHittedMode() { return Type == EStateType::Hitted; }
	FORCEINLINE bool IsDeadMode() { return Type == EStateType::Dead; }
	FORCEINLINE bool IsActionMode() { return Type == EStateType::Action; }
	FORCEINLINE bool IsSubActionMode() { return bInSubActionMode; }

public:
	UCStateComponent();

protected:
	virtual void BeginPlay() override;

public:
	void SetIdleMode();
	void SetBackStepMode();
	void SetEquipMode();
	void SetHittedMode();
	void SetDeadMode();
	void SetActionMode();
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
};