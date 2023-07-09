#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Components/CStateComponent.h"
#include "CBaseCharacter.generated.h"

class ACWeapon;
class UAnimMontage;

UCLASS()
class RPG_API ACBaseCharacter : public ACharacter
{
	GENERATED_BODY()

protected:
	UPROPERTY(VisibleAnywhere)
		class UCWeaponComponent* Weapon;

	UPROPERTY(VisibleAnywhere)
		class UCMontagesComponent* Montages;

	UPROPERTY(VisibleAnywhere)
		class UCMovementComponent* Movement;

	UPROPERTY(VisibleAnywhere)
		class UCStateComponent* State;

	UPROPERTY(VisibleAnywhere)
		class UCStatusComponent* Status;

	//���� ���� �� ������ ���� ����
	UPROPERTY(VisibleAnywhere, Category = Weapon)
	ACWeapon* EquippedWeapon;

public:
	ACBaseCharacter();

protected:
	virtual void BeginPlay() override;

};
