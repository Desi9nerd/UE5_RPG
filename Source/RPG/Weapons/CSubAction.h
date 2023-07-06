#pragma once
#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "CSubAction.generated.h"

UCLASS(Abstract)//객체화하지 않는 경우, Abstract 명시
class RPG_API UCSubAction : public UObject
{
	GENERATED_BODY()

public:
	FORCEINLINE bool GetInAction() { return bInAction; }//bInAction의 true,false 여부를 리턴하는 함수

public:
	UCSubAction();

public:
	virtual void BeginPlay(class ACharacter* InOwner, class ACAttachment* InAttachment, class UCDoAction* InDoAction);

public:
	virtual void Pressed() {}
	virtual void Released() {}

public:
	//NativeEvent는 가상화. 정의할테니 원하면 가져다가 써라는 의미.
	//Implementation는 추상화. 함수 호출 해줄테니 필요하면 재정의해서 써라.
	UFUNCTION(BlueprintNativeEvent)
		void Begin_SubAction();
	virtual void Begin_SubAction_Implementation();

	UFUNCTION(BlueprintNativeEvent)
		void End_SubAction();
	virtual void End_SubAction_Implementation();

	UFUNCTION(BlueprintNativeEvent)
		void Tick(float InDeltaTime);
	virtual void Tick_Implementation(float InDeltaTime) {}

protected:
	bool bInAction;

	class ACharacter* Owner;
	class ACAttachment* Attachment;
	class UCDoAction* DoAction;

	class UCStateComponent* State;
	class UCMovementComponent* Movement;
};