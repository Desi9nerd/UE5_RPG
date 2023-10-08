#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Characters/ECharacterTypes.h"
#include "Characters/CBaseCharacter.h"
#include "Parkour/CParkourComponent.h"
#include "Interfaces/IPickup.h"
#include "Weapons/CAttachment.h"//Pickup
#include "Weapons/Attachments/CAttachment_Bow.h"//Pickup
#include "Components/SplineComponent.h"
#include "CPlayer.generated.h"

UCLASS()
class RPG_API ACPlayer
	: public ACBaseCharacter, public IIPickup //다중상속
{
	GENERATED_BODY()

private:
	UPROPERTY(VisibleAnywhere)
	class USpringArmComponent* SpringArm;
	
	UPROPERTY(VisibleAnywhere)
	class UCameraComponent* Camera;

/** 파쿠르 */
	UPROPERTY(VisibleDefaultsOnly)
	class USceneComponent* ArrowGroup;//파쿠르를 위한 ArrowGroup

	UPROPERTY(VisibleDefaultsOnly)
	class UArrowComponent* Arrows[(int32)EParkourArrowType::Max];

	UPROPERTY(VisibleDefaultsOnly)
	class UCParkourComponent* Parkour;
/** 파쿠르 */

	UPROPERTY(VisibleDefaultsOnly)
	class UCZoomComponent* Zoom;

	UPROPERTY(VisibleDefaultsOnly)
	class UCPlayerOverlay* PlayerOverlay;
	
	UPROPERTY(VisibleDefaultsOnly)
	class UCPromptText* PromptTextWidget;

	APlayerController* PlayerController;//PlayerController 변수

public:
	ACPlayer();
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	void OnStateTypeChanged(EStateType InPrevType, EStateType InNewType) override;

	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;//TakeDamage 오버라이드하여 재정의.

	void End_Dodge() override;//ICharacter의 함수 오버라이드
	void Landed(const FHitResult& Hit) override;//BP의 OnLanded C++버젼.

	void OnRightButton();
	void OffRightButton();
	void MiddleMouse_Pressed();
	void MiddleMouse_Released();

protected:
	virtual void BeginPlay() override;
	void Tick(float DeltaSeconds) override;
	
private:
	void OnAvoid();
	void Dodge();	
	void SetZooming(float InValue);
	void InitializePlayerOverlay(); // PlayerOverlay Widget
	void SetHUDHealth(); // HUD
};