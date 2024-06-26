#include "CStatusComponent.h"
#include "Global.h"
#include "GameFramework/Character.h"

void UCStatusComponent::BeginPlay()
{
	Super::BeginPlay();

	OwnerCharacter = Cast<ACharacter>(GetOwner());

	//���� ���� �� ü��, ���׹̳� Max�� ����.
	Health = MaxHealth;
	Stamina = MaxStamina;
}

void UCStatusComponent::RegenStamina(float DeltaTime)
{
	Stamina = FMath::Clamp(Stamina + StaminaRegenRate * DeltaTime, 0.f, MaxStamina);
}

void UCStatusComponent::UseStamina(float StaminaCost)
{
	Stamina = FMath::Clamp(Stamina - StaminaCost, 0.0f, MaxStamina);
}

void UCStatusComponent::Damage(float InAmount)
{
	Health += (InAmount * -1.0f);
	Health = FMath::Clamp(Health, 0.0f, MaxHealth);
}
