#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotifyState.h"
#include "NiagaraSystem.h"
#include "CAnimNotifyState_Collision.generated.h"

UCLASS()
class RPG_API UCAnimNotifyState_Collision : public UAnimNotifyState
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere)
	FName StartSocketName;

	UPROPERTY(EditAnywhere)
	FName EndSocketName;

	UPROPERTY(EditAnywhere)
	UParticleSystem* HitEffectBlood;

	UPROPERTY(EditAnywhere)
	UParticleSystem* HitEffectParticleSystem;

	UPROPERTY(EditAnywhere)
	UNiagaraSystem* HitEffectNiagaraSystem;

	UPROPERTY(EditAnywhere)
	FVector ParticleScale = FVector(1.0f, 1.0f, 1.0f);

	UPROPERTY(EditAnywhere)
	FVector NiagaraScale = FVector(1.0f, 1.0f, 1.0f);

	FString GetNotifyName_Implementation() const override;

	virtual void NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration, const FAnimNotifyEventReference& EventReference) override;
	virtual void NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference) override;
	virtual void NotifyTick(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float FrameDeltaTime) override;
};