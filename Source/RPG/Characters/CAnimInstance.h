#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "Components/CWeaponComponent.h"
#include "Components/CFeetComponent.h"
#include "CAnimInstance.generated.h"

UCLASS()
class RPG_API UCAnimInstance : public UAnimInstance
{
	GENERATED_BODY()

protected:
    UPROPERTY(BlueprintReadOnly, Category = "Animation")
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

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Animation")
        bool IsFalling;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Animation")
		int JumpCnt;

    //Feet IK
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "InverseKinemetics")
        bool bFeet;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "InverseKinemetics")
        FFeetData FeetData;
    
	//Parkour
    UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Animation")
        EWeaponType WeaponType = EWeaponType::Max;

public:
    void NativeBeginPlay() override;
    void NativeUpdateAnimation(float DeltaSeconds) override;

private:
    UFUNCTION()
        void OnWeaponTypeChanged(EWeaponType InPrevType, EWeaponType InNewType);

private:
    class ACharacter* OwnerCharacter;
    class UCWeaponComponent* Weapon;
    class UCMovementComponent* Movement;

private:
    FRotator PrevRotation;
    
};
