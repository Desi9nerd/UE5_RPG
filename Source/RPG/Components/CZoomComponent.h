#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "CZoomComponent.generated.h"


UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class RPG_API UCZoomComponent : public UActorComponent
{
	GENERATED_BODY()

private:
	UPROPERTY(EditAnywhere, Category = "Zooming")
		float Speed = 20;

	UPROPERTY(EditAnywhere, Category = "Zooming")
		float InterpSpeed = 5;

	UPROPERTY(EditAnywhere, Category = "Zooming")
		FVector2D Range = FVector2D(100, 500);

public:
	UCZoomComponent();

	void SetZoomValue(float InValue);

protected:
	virtual void BeginPlay() override;

public:
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

private:
	class USpringArmComponent* SpringArm;

	float CurrentValue;

};
