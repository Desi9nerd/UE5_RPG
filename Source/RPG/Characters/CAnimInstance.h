#pragma once
#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "Components/CWeaponComponent.h"
#include "Components/CFeetComponent.h"
#include "CAnimInstance.generated.h"

class UCWeaponComponent;
class UCMovementComponent;
class ACharacter;

UCLASS()
class RPG_API UCAnimInstance : public UAnimInstance
{
	GENERATED_BODY()

public:
    void NativeInitializeAnimation() override;
    void NativeUpdateAnimation(float DeltaSeconds) override;

protected:
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation")
	float Speed;

    UPROPERTY(BlueprintReadOnly, Category = "Animation")
	float Pitch;

    UPROPERTY(BlueprintReadOnly, Category = "Animation")
	float Direction;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Animation")
	bool bFalling;//추락중 여부

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Animation")
	bool bBow_Aiming;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Animation")
	class UCharacterMovementComponent* CharacterMovement;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation")
	bool IsFalling;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Animation")
	int JumpCnt;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Animation")
	ACharacter* OwnerCharacter;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Animation")
	UCMovementComponent* Movement;

    //*****************************************************
    //Parkour
    UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Animation")
        EWeaponType WeaponType = EWeaponType::Max;
    //*****************************************************

    //*****************************************************
    //Feet IK
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "InverseKinemetics")
	bool bFeet;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "InverseKinemetics")
	FFeetData FeetData;
    //*****************************************************

    //*****************************************************
	//** Bow
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Bow")
	FVector Velocity;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Bow")
	bool ShouldMove;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Bow")
	FRotator BaseAimRotation;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Bow")
	float YawOffset;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Bow")
	float RootYawOffset;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Bow")
	FRotator MovingRotation;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Bow")
	FRotator LastMovingRotation;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Bow")
	float DistanceCurve;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Bow")
	float LastDistanceCurve;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Bow")
    float DeltaDistanceCurve;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Bow")
    float AbsRootYawOffset;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Bow")
    float YawExcess;
    //*****************************************************

private:
    UFUNCTION()
	void OnWeaponTypeChanged(EWeaponType InPrevType, EWeaponType InNewType);

    UCWeaponComponent* Weapon;

    FRotator PrevRotation;
};