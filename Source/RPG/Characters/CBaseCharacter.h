#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Interfaces/IHit.h"
#include "Characters/ECharacterTypes.h"
#include "Components/CStateComponent.h"
#include "CBaseCharacter.generated.h"

class UAnimMontage;

UCLASS()
class RPG_API ACBaseCharacter : public ACharacter, public IIHit
{
	GENERATED_BODY()

protected:
	UPROPERTY(VisibleAnywhere)
		class UCMontagesComponent* Montages;

	UPROPERTY(VisibleAnywhere)
		class UCMovementComponent* Movement;

	UPROPERTY(VisibleAnywhere)
		class UCStateComponent* State;

	UPROPERTY(VisibleAnywhere)
		class UCStatusComponent* Status;

	//무기 장착 및 해제를 위한 변수
	//UPROPERTY(VisibleAnywhere, Category = Weapon)
	//ACWeapon* EquippedWeapon;

public:
	ACBaseCharacter();

protected:
	virtual void BeginPlay() override;
};
