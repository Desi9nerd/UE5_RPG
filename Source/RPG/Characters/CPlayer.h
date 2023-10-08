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
	: public ACBaseCharacter, public IIPickup //���߻��
{
	GENERATED_BODY()

private:
	UPROPERTY(VisibleAnywhere)
	class USpringArmComponent* SpringArm;
	
	UPROPERTY(VisibleAnywhere)
	class UCameraComponent* Camera;

/** ���� */
	UPROPERTY(VisibleDefaultsOnly)
	class USceneComponent* ArrowGroup;//������ ���� ArrowGroup

	UPROPERTY(VisibleDefaultsOnly)
	class UArrowComponent* Arrows[(int32)EParkourArrowType::Max];

	UPROPERTY(VisibleDefaultsOnly)
	class UCParkourComponent* Parkour;
/** ���� */

	UPROPERTY(VisibleDefaultsOnly)
	class UCZoomComponent* Zoom;

	UPROPERTY(VisibleDefaultsOnly)
	class UCPlayerOverlay* PlayerOverlay;
	
	UPROPERTY(VisibleDefaultsOnly)
	class UCPromptText* PromptTextWidget;

	APlayerController* PlayerController;//PlayerController ����

public:
	ACPlayer();
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	void OnStateTypeChanged(EStateType InPrevType, EStateType InNewType) override;

	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;//TakeDamage �������̵��Ͽ� ������.

	void End_Dodge() override;//ICharacter�� �Լ� �������̵�
	void Landed(const FHitResult& Hit) override;//BP�� OnLanded C++����.

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