#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotifyState.h"
#include "CAnimNotifyState.generated.h"

UCLASS()
class RPG_API UCAnimNotifyState : public UAnimNotifyState
{
	GENERATED_BODY()

private:
	UPROPERTY(EditAnywhere, Category = "Camera")
		class UCameraAnim* CameraAnim;

	UPROPERTY(EditAnywhere, Category = "Camera")
		float PlayRatio = 1.0f;

	UPROPERTY(EditAnywhere, Category = "Camera")
		float BlendInTime = 0.0f;//���� ī�޶󿡼� Animation ī�޶�� ��ȯ�Ǵ� �ð�

	UPROPERTY(EditAnywhere, Category = "Camera")
		float BlendOutTime = 0.0f;//Animation ī�޶󿡼� ���� ī�޶�� ��ȯ�Ǵ� �ð�

};
