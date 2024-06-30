#pragma once
#include "CoreMinimal.h"
#include "CSubAction.generated.h"

class UCStateComponent;
class UCMovementComponent;

UCLASS(Abstract)//객체화하지 않는 경우, Abstract 명시
class RPG_API UCSubAction : public UObject
{
	GENERATED_BODY()

public:
	FORCEINLINE bool GetInAction() { return bInAction; }
	
	virtual void BeginPlay(class ACharacter* InOwner, class ACAttachment* InAttachment, class UCDoAction* InDoAction);
	
	virtual void Pressed();
	virtual void Released();
	
	UFUNCTION(BlueprintNativeEvent)
	void Begin_SubAction();
	virtual void Begin_SubAction_Implementation() {}

	UFUNCTION(BlueprintNativeEvent)
	void End_SubAction();
	virtual void End_SubAction_Implementation() {}

	UFUNCTION(BlueprintNativeEvent)
	void Tick(float InDeltaTime);
	virtual void Tick_Implementation(float InDeltaTime) {}

protected:
	bool bInAction;

	ACharacter* Owner;
	ACAttachment* Attachment;
	UCDoAction* DoAction;

	UCStateComponent* State;
	UCMovementComponent* Movement;
};
