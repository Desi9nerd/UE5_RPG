#pragma once
#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotifyState.h"
#include "CAnimNotifyState_CameraAnim.generated.h"

class UCameraAnim;
class UCameraModifier;
class UCCameraModifier;

UCLASS()
class RPG_API UCAnimNotifyState_CameraAnim : public UAnimNotifyState
{
	GENERATED_BODY()

public:
	UCAnimNotifyState_CameraAnim();//������
	
	FString GetNotifyName_Implementation() const override;
	
	virtual void NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration, const FAnimNotifyEventReference& EventReference) override;
	virtual void NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference) override;
	
private:
	UPROPERTY(EditAnywhere, Category = "Camera")
	UCameraAnim* CameraAnim;

	UPROPERTY(EditAnywhere, Category = "Camera")
	float PlayRatio = 1.0f;

	UPROPERTY(EditAnywhere, Category = "Camera")
	float BlendInTime = 0.0f;//���� ī�޶󿡼� Animation ī�޶�� ��ȯ�Ǵ� �ð�

	UPROPERTY(EditAnywhere, Category = "Camera")
	float BlendOutTime = 0.0f;//Animation ī�޶󿡼� ���� ī�޶�� ��ȯ�Ǵ� �ð�

	UPROPERTY(EditAnywhere, Category = "Camera")
	TSoftObjectPtr<ACharacter> Target;

	UPROPERTY(EditAnywhere, Category = "Modifier")
	TSubclassOf<UCameraModifier> ModifierClass;

	UPROPERTY(EditAnywhere, Category = "Modifier")
	FVector LocationOffset = FVector(-400.f, 0.f, 90.f);

	UPROPERTY(EditAnywhere, Category = "Modifier")
	float PitchOffset = -15.f;

	UCCameraModifier* Modifier;
};
