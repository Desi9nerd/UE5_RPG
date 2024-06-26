#pragma once
#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Characters/ECharacterTypes.h"
#include "Characters/CBaseCharacter.h"
#include "Parkour/CParkourComponent.h"
#include "Weapons/Attachments/CAttachment_Bow.h"
#include "CPlayer.generated.h"

class USpringArmComponent;
class UCameraComponent;
class USceneComponent;
class UCZoomComponent;
class UCPlayerOverlay;
class UCPromptText;

UCLASS()
class RPG_API ACPlayer
	: public ACBaseCharacter
{
	GENERATED_BODY()

public:
	ACPlayer();
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	void OnStateTypeChanged(EStateType InPrevType, EStateType InNewType) override;

	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;
	void Landed(const FHitResult& Hit) override;

	void OnRightButton();
	void OffRightButton();
	void MiddleMouse_Pressed();
	void MiddleMouse_Released();

	void End_Dodge() override;//ICharacter의 함수 오버라이드

protected:
	virtual void BeginPlay() override;
	void Tick(float DeltaSeconds) override;
	
private:
	void OnAvoid();
	void Dodge();	
	void SetZooming(float InValue);
	void InitializePlayerOverlay(); // PlayerOverlay Widget
	void SetHUDHealth(); // HUD
	
	UPROPERTY(VisibleAnywhere)
	USpringArmComponent* SpringArm;

	UPROPERTY(VisibleAnywhere)
	UCameraComponent* Camera;

	UPROPERTY(VisibleDefaultsOnly)
	UCZoomComponent* Zoom;

	UPROPERTY(VisibleDefaultsOnly)
	UCPlayerOverlay* PlayerOverlay;

	UPROPERTY(VisibleDefaultsOnly)
	UCPromptText* PromptTextWidget;

	APlayerController* PlayerController;

	//********************************************************
	//** Parkour
	UPROPERTY(VisibleDefaultsOnly)
	USceneComponent* ArrowGroup;//파쿠르를 위한 ArrowGroup

	UPROPERTY(VisibleDefaultsOnly)
	UArrowComponent* Arrows[(int32)EParkourArrowType::Max];

	UPROPERTY(VisibleDefaultsOnly)
	UCParkourComponent* Parkour;
	//********************************************************
};