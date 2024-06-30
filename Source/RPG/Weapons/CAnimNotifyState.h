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
	float BlendInTime = 0.f;//���� ī�޶󿡼� Animation ī�޶�� ��ȯ�Ǵ� �ð�

	UPROPERTY(EditAnywhere, Category = "Camera")
	float BlendOutTime = 0.f;//Animation ī�޶󿡼� ���� ī�޶�� ��ȯ�Ǵ� �ð�
};
