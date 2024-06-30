#pragma once
#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotifyState.h"
#include "CAnimNotifyState.generated.h"

class UCameraAnim;

UCLASS()
class RPG_API UCAnimNotifyState : public UAnimNotifyState
{
	GENERATED_BODY()

private:
	UPROPERTY(EditAnywhere, Category = "Camera")
	UCameraAnim* CameraAnim;

	UPROPERTY(EditAnywhere, Category = "Camera")
	float PlayRatio = 1.f;

	UPROPERTY(EditAnywhere, Category = "Camera")
	float BlendInTime = 0.f;//원래 카메라에서 Animation 카메라로 전환되는 시간

	UPROPERTY(EditAnywhere, Category = "Camera")
	float BlendOutTime = 0.f;//Animation 카메라에서 원래 카메라로 전환되는 시간
};
