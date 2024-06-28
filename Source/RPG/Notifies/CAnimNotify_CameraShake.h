#pragma once
#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "MatineeCameraShake.h"
#include "CAnimNotify_CameraShake.generated.h"

class UMatineeCameraShake;

UCLASS()
class RPG_API UCAnimNotify_CameraShake : public UAnimNotify
{
	GENERATED_BODY()

public:
	FString GetNotifyName_Implementation() const override;//Notify 이름을 지어주는 함수.

	void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference) override;

private:
	UPROPERTY(EditAnywhere, Category = "CameraClass")
	TSubclassOf<UMatineeCameraShake> CameraShakeClass;
};