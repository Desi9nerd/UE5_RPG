#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "CTargetComponent.generated.h"

class UWidgetComponent;
class UCMovementComponent;
class UCStateComponent;
class ACBaseCharacter;
class AController;

UCLASS(Blueprintable)
class RPG_API UCTargetComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UCTargetComponent();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UFUNCTION(BlueprintCallable)
	void Toggle_Target();

	UFUNCTION(BlueprintCallable)
	void ChangeFocus(bool InbRight);

	void Begin_Targeting();
	void End_Targeting();
	void ChangeTarget(ACBaseCharacter* InCandidate);
	void Tick_Targeting();

private:
	TWeakObjectPtr<ACBaseCharacter> NearlyForward(TWeakObjectPtr<ACBaseCharacter> InCenterTarget,
		TArray<TWeakObjectPtr<ACBaseCharacter>> InArray,
		TWeakObjectPtr<AController> InController);

	void AddNearSideCharacters(
		TWeakObjectPtr<ACBaseCharacter> InCenter,
		TArray<TWeakObjectPtr<ACBaseCharacter>> InArray,
		TWeakObjectPtr<AController> InController,
		TMap<float, TWeakObjectPtr<ACBaseCharacter>>& OutNearCharacters);

protected:
	UPROPERTY(EditAnywhere, Category = "Components")
	class UParticleSystemComponent* Particle;

	UPROPERTY(EditAnywhere, Category = "Settings")
	float TraceDistance = 1200.0f;

	UPROPERTY(EditAnywhere, Category = "Settings")
	UParticleSystem* ParticleAsset;	

	UPROPERTY(EditAnywhere, Category = "Settings")
	float FinishAngle = 0.1f;

	UPROPERTY(EditAnywhere, Category = "Settings")
	float InterpSpeed = 20.0f;

public:
	UPROPERTY(VisibleAnywhere, Category = "Focus")
	bool bTargeting;

	UPROPERTY(VisibleAnywhere, Category = "Focus")
	bool bMovingFocus;

private:
	TWeakObjectPtr<ACBaseCharacter> Character;
	TWeakObjectPtr<ACBaseCharacter> Target;
	TWeakObjectPtr<AController> Controller;
	TWeakObjectPtr<UCMovementComponent> TargetCameraComp;
};
