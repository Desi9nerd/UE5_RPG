#include "Components/CZoomComponent.h"
#include "Global.h"
#include "GameFramework/SpringArmComponent.h"

UCZoomComponent::UCZoomComponent()
{
	PrimaryComponentTick.bCanEverTick = true;

}

void UCZoomComponent::BeginPlay()
{
	Super::BeginPlay();

	SpringArm = CHelpers::GetComponent<USpringArmComponent>(GetOwner());
	CheckNull(SpringArm);

	CurrentValue = SpringArm->TargetArmLength;
}

void UCZoomComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	CheckNull(SpringArm);

	CheckTrue(UKismetMathLibrary::NearlyEqual_FloatFloat(SpringArm->TargetArmLength, CurrentValue));
	SpringArm->TargetArmLength = UKismetMathLibrary::FInterpTo(SpringArm->TargetArmLength, CurrentValue, DeltaTime, InterpSpeed);
}

void UCZoomComponent::SetZoomValue(float InValue)
{
	CurrentValue += (Speed * InValue);
	CurrentValue = FMath::Clamp(CurrentValue, Range.X, Range.Y);
}