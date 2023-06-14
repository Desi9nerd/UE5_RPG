#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "CWeaponStructures.generated.h"

USTRUCT()
struct FEquipmentData
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere)
		class UAnimMontage* Montage;

	UPROPERTY(EditAnywhere)
		float PlayRate = 1;

	UPROPERTY(EditAnywhere)
		bool bCanMove = true;

	UPROPERTY(EditAnywhere)
		bool bUseControlRotation = true;
};

USTRUCT()
struct FDoActionData
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere)
		class UAnimMontage* Montage;

	UPROPERTY(EditAnywhere)
		float PlayRate = 1;

	UPROPERTY(EditAnywhere)
		bool bCanMove = true;

	UPROPERTY(EditAnywhere)
		bool bFixedCamera;

	UPROPERTY(EditAnywhere)
		class UFXSystemAsset* Effect; //사용할 Effect 변수

	UPROPERTY(EditAnywhere)
		FVector EffectLocation = FVector::ZeroVector;//(Effect)지정 방향의 보정치.

	UPROPERTY(EditAnywhere)
		FVector EffectScale = FVector::OneVector;//Effect 크기 기본값 1 설정.

public:
	void DoAction(class ACharacter* InOwner);

	void PlayEffect(UWorld* InWorld, const FVector& InLocation);
	void PlayEffect(UWorld* InWorld, const FVector& InLocation, const FRotator& InRotation);
};

USTRUCT()
struct FHitData
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere)
		class UAnimMontage* Montage;

	UPROPERTY(EditAnywhere)
		float PlayRate = 1;

	UPROPERTY(EditAnywhere)
		float Power;

	UPROPERTY(EditAnywhere)
		float Launch = 100;

	UPROPERTY(EditAnywhere)
		float StopTime;

	UPROPERTY(EditAnywhere)
		class USoundWave* Sound;

	UPROPERTY(EditAnywhere)
		class UFXSystemAsset* Effect;

	UPROPERTY(EditAnywhere)
		FVector EffectLocation = FVector::ZeroVector;

	UPROPERTY(EditAnywhere)
		FVector EffectScale = FVector::OneVector;

public:
	void SendDamage(class ACharacter* InAttacker, AActor* InAttackCauser, class ACharacter* InOther);
	void PlayMontage(class ACharacter* InOwner);
	void PlayHitStop(UWorld* InWorld);
	void PlaySoundWave(class ACharacter* InOwner);
	void PlayEffect(UWorld* InWorld, const FVector& InLocation);
	void PlayEffect(UWorld* InWorld, const FVector& InLocation, const FRotator& InRotation);
};

USTRUCT()
struct FActionDamageEvent : public FDamageEvent
{
	GENERATED_BODY()

public:
	FHitData* HitData;
};


UCLASS()
class RPG_API UCWeaponStructures : public UObject
{
	GENERATED_BODY()
	
};
