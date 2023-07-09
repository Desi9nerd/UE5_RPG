#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Components/CStateComponent.h"
#include "Characters/CBaseCharacter.h"
#include "Characters/ICharacter.h"
#include "Interfaces/IPickup.h"
#include "CPlayer.generated.h"

UCLASS()
class RPG_API ACPlayer
	: public ACBaseCharacter, public IICharacter, public IIPickup //���߻��
{
	GENERATED_BODY()

private:
	UPROPERTY(VisibleAnywhere)
		class USpringArmComponent* SpringArm;

	UPROPERTY(VisibleAnywhere)
		class UCameraComponent* Camera;
	
public:
	ACPlayer();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

private:
	UFUNCTION()
		void OnStateTypeChanged(EStateType InPrevType, EStateType InNewType);

private:
	void OnAvoid();

private:
	void BackStep();

public:
	void End_BackStep() override;//ICharacter�� �Լ� �������̵�
};
