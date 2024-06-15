#pragma once
#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "CZoomComponent.generated.h"

class USpringArmComponent;

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class RPG_API UCZoomComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UCZoomComponent();
	void SetZoomValue(float InValue);
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

protected:
	virtual void BeginPlay() override;
	
private:
	USpringArmComponent* SpringArm;
	float CurrentValue;

	UPROPERTY(EditAnywhere, Category = "Zooming", meta = (AllowPrivateAccess = true))
	float Speed = 20;

	UPROPERTY(EditAnywhere, Category = "Zooming", meta = (AllowPrivateAccess = true))
	float InterpSpeed = 5;

	UPROPERTY(EditAnywhere, Category = "Zooming", meta = (AllowPrivateAccess = true))
	FVector2D Range = FVector2D(100, 500);
};
