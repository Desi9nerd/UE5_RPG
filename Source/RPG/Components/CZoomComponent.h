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
	UPROPERTY(EditAnywhere, Category = "Zooming", meta = (AllowPrivateAccess = true))
	float Speed = 20.f;

	UPROPERTY(EditAnywhere, Category = "Zooming", meta = (AllowPrivateAccess = true))
	float InterpSpeed = 5.f;

	UPROPERTY(EditAnywhere, Category = "Zooming", meta = (AllowPrivateAccess = true))
	FVector2D Range = FVector2D(100, 500);

	USpringArmComponent* SpringArm;
	float CurrentValue;
};
