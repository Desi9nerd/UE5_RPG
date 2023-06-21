#pragma once

#include "CoreMinimal.h"
#include "Camera/CameraModifier.h"
#include "CCameraModifier.generated.h"

UCLASS()
class RPG_API UCCameraModifier : public UCameraModifier
{
	GENERATED_BODY()

public:
	//�ܺο��� ���� �־��� �� �ְ� FORCEINLINE �Լ� ���.
	FORCEINLINE void SetLocation(const FVector& InLocation) { Location = InLocation; }
	FORCEINLINE void SetRotation(const FRotator& InRotation) { Rotation = InRotation; }

protected:
	void ModifyCamera(float DeltaTime, FVector ViewLocation, FRotator ViewRotation, float FOV, FVector& NewViewLocation, FRotator& NewViewRotation, float& NewFOV) override;

private:
	FVector Location;
	FRotator Rotation;
};
