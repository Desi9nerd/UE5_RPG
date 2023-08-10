#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "CStatusComponent.generated.h"


UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class RPG_API UCStatusComponent : public UActorComponent
{
    GENERATED_BODY()

public:
    UCStatusComponent();

protected:
    virtual void BeginPlay() override;

private:
    UPROPERTY(EditAnywhere, Category = "Attributes")
        float MaxHealth = 100;

    UPROPERTY(EditAnywhere, Category = "Attributes")
        float MaxStamina = 100;

    UPROPERTY(EditAnywhere, Category = "Attributes")
        float StaminaRegenRate = 5.0f;

    UPROPERTY(EditAnywhere, Category = "Attributes")
        float DodgeCost = 20.0f;

public:
    FORCEINLINE float GetHealth() { return Health; }
    FORCEINLINE float GetMaxHealth() { return MaxHealth; }
    FORCEINLINE bool IsDead() { return Health <= 0.0f; }

    FORCEINLINE float GetHealthPercent() { return Health / MaxHealth; }

    FORCEINLINE float GetStamina() const { return Stamina; }
    FORCEINLINE float GetStaminaPercent() { return Stamina / MaxStamina; }
    FORCEINLINE float GetDodgeCost() const { return DodgeCost; }

    void RegenStamina(float DeltaTime);
    void UseStamina(float StaminaCost);

public:
    void Damage(float InAmount);

private:
    class ACharacter* OwnerCharacter;

private:
    float Health;
    float Stamina;
};