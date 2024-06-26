#pragma once
#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "CStatusComponent.generated.h"

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class RPG_API UCStatusComponent : public UActorComponent
{
    GENERATED_BODY()

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
    void Damage(float InAmount);

protected:
    virtual void BeginPlay() override;

private:
    UPROPERTY(EditAnywhere, Category = "Attributes")
	float MaxHealth = 100;

    UPROPERTY(EditAnywhere, Category = "Attributes")
	float MaxStamina = 100;

    UPROPERTY(EditAnywhere, Category = "Attributes")
	float StaminaRegenRate = 5.f;

    UPROPERTY(EditAnywhere, Category = "Attributes")
	float DodgeCost = 20.f;

    ACharacter* OwnerCharacter;
    float Health;
    float Stamina;
};