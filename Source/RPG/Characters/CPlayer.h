#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Characters/ECharacterTypes.h"
#include "Components/CStateComponent.h"
#include "Characters/CBaseCharacter.h"
#include "Characters/ICharacter.h"
#include "Parkour/CParkourComponent.h"
#include "GenericTeamAgentInterface.h"
#include "Interfaces/IPickup.h"
#include "Weapons/CAttachment.h"//Pickup
#include "Weapons/Attachments/CAttachment_Bow.h"//Pickup
#include "Components/SplineComponent.h"
#include "CPlayer.generated.h"

class ACItem;

UCLASS()
class RPG_API ACPlayer
	: public ACBaseCharacter, public IICharacter, public IGenericTeamAgentInterface, public IIPickup //다중상속
{
	GENERATED_BODY()

private:
	UPROPERTY(EditDefaultsOnly, Category = "Team")
		uint8 TeamID = 1;

protected:
	UPROPERTY(VisibleAnywhere)
		class UCWeaponComponent* Weapon;

private:
	UPROPERTY(VisibleAnywhere)
		class USpringArmComponent* SpringArm;

	UPROPERTY(VisibleAnywhere)
		class UCameraComponent* Camera;

/** 파쿠르 */
private:
	UPROPERTY(VisibleDefaultsOnly)
		class USceneComponent* ArrowGroup;//파쿠르를 위한 ArrowGroup

	UPROPERTY(VisibleDefaultsOnly)
		class UArrowComponent* Arrows[(int32)EParkourArrowType::Max];

	UPROPERTY(VisibleDefaultsOnly)
		class UCParkourComponent* Parkour;
/** 파쿠르 */

	UPROPERTY(VisibleDefaultsOnly)
		class UCZoomComponent* Zoom;

public:
	UPROPERTY(VisibleDefaultsOnly)
		USplineComponent* ArrowPathSpline;

public:
	ACPlayer();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	FGenericTeamId GetGenericTeamId() const override { return FGenericTeamId(TeamID); }

private:
	UFUNCTION()
		void OnStateTypeChanged(EStateType InPrevType, EStateType InNewType);

private:
	void OnAvoid();

private:
	void Dodge();

public:
	void End_Dodge() override;//ICharacter의 함수 오버라이드


public:
	//무기 줍기
	void FKeyPressed();
	virtual void SetOverlappingItem(ACItem* Item) override;

	UPROPERTY(VisibleInstanceOnly)
	ACItem* OverlappingItem;

public:
	void Click_RightButton();

public:
	void Landed(const FHitResult& Hit) override;//BP의 OnLanded C++버젼.

private:
	void SetZooming(float InValue);
};
