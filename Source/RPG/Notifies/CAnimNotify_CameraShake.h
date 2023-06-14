#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "CAnimNotify_CameraShake.generated.h"

UCLASS()
class RPG_API UCAnimNotify_CameraShake : public UAnimNotify
{
	GENERATED_BODY()

private:
	UPROPERTY(EditAnywhere, Category = "CameraClass")
		TSubclassOf<class UMatineeCameraShake> CameraShakeClass;

public:
	FString GetNotifyName_Implementation() const override;//Notify 이름을 지어주는 함수.

	void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation) override;
};