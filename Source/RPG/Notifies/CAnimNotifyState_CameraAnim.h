#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotifyState.h"
#include "CAnimNotifyState_CameraAnim.generated.h"

UCLASS()
class RPG_API UCAnimNotifyState_CameraAnim : public UAnimNotifyState
{
	GENERATED_BODY()

public:
	UCAnimNotifyState_CameraAnim();//������
	
private:
	UPROPERTY(EditAnywhere, Category = "Camera")
		class UCameraAnim* CameraAnim;

	UPROPERTY(EditAnywhere, Category = "Camera")
		float PlayRatio = 1.0f;

	UPROPERTY(EditAnywhere, Category = "Camera")
		float BlendInTime = 0.0f;//���� ī�޶󿡼� Animation ī�޶�� ��ȯ�Ǵ� �ð�

	UPROPERTY(EditAnywhere, Category = "Camera")
		float BlendOutTime = 0.0f;//Animation ī�޶󿡼� ���� ī�޶�� ��ȯ�Ǵ� �ð�

	UPROPERTY(EditAnywhere, Category = "Camera")
		TSoftObjectPtr<class ACharacter> Target;

	UPROPERTY(EditAnywhere, Category = "Modifier")
		TSubclassOf<class UCameraModifier> ModifierClass;

	UPROPERTY(EditAnywhere, Category = "Modifier")
		FVector LocationOffset = FVector(-400, 0, 90);

	UPROPERTY(EditAnywhere, Category = "Modifier")
		float PitchOffset = -15.0f;

public:
	FString GetNotifyName_Implementation() const override;
	
	virtual void NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration, const FAnimNotifyEventReference& EventReference) override;

	virtual void NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference) override;

private:
	class UCCameraModifier* Modifier;
};
