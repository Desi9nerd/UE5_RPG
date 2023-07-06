#pragma once
#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "CSubAction.generated.h"

UCLASS(Abstract)//��üȭ���� �ʴ� ���, Abstract ���
class RPG_API UCSubAction : public UObject
{
	GENERATED_BODY()

public:
	FORCEINLINE bool GetInAction() { return bInAction; }//bInAction�� true,false ���θ� �����ϴ� �Լ�

public:
	UCSubAction();

public:
	virtual void BeginPlay(class ACharacter* InOwner, class ACAttachment* InAttachment, class UCDoAction* InDoAction);

public:
	virtual void Pressed() {}
	virtual void Released() {}

public:
	//NativeEvent�� ����ȭ. �������״� ���ϸ� �����ٰ� ���� �ǹ�.
	//Implementation�� �߻�ȭ. �Լ� ȣ�� �����״� �ʿ��ϸ� �������ؼ� ���.
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