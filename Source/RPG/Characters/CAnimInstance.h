#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "Components/CWeaponComponent.h"
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

    UPROPERTY(BlueprintReadOnly, Category = "Animation")
        bool bBow_Aiming;

    UPROPERTY(BlueprintReadOnly, Category = "Animation")
        class UCharacterMovementComponent* CharacterMovement;

    UPROPERTY(BlueprintReadOnly, Category = "Animation")
        bool IsFalling;

    UPROPERTY(BlueprintReadOnly, Category = "Animation")
		int JumpCnt;

protected:
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
