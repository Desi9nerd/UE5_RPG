#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "ICharacter.h"
#include "Interfaces/IHit.h"
#include "Characters/ECharacterTypes.h"
#include "Components/CStateComponent.h"
#include "CBaseCharacter.generated.h"

class UAnimMontage;

UCLASS()
class RPG_API ACBaseCharacter
	: public ACharacter, public IIHit, public IICharacter //다중상속
{
	GENERATED_BODY()

protected:
	UPROPERTY(VisibleAnywhere)
		class UCWeaponComponent* Weapon;

	UPROPERTY(VisibleAnywhere)
		class UCMontagesComponent* Montages;

	UPROPERTY(VisibleAnywhere)
		class UCMovementComponent* Movement;

	UPROPERTY(VisibleAnywhere)
		class UCStateComponent* State;

	UPROPERTY(VisibleAnywhere)
		class UCStatusComponent* Status;

	UPROPERTY(VisibleAnywhere)
		class UTextRenderComponent* TextRender_State;
	UPROPERTY(VisibleAnywhere)
		class UTextRenderComponent* TextRender_Weapon;

	//무기 장착 및 해제를 위한 변수
	//UPROPERTY(VisibleAnywhere, Category = Weapon)
	//ACWeapon* EquippedWeapon;

	UPROPERTY(EditAnywhere, Category = "Color")
		FLinearColor OriginColor = FLinearColor::White;//색상 설정.

	FTimerHandle RestoreColor_TimerHandle;//변한 색이 일정시간 후에 되돌아오도록 시간을 기록하는 변수.

public:
	ACBaseCharacter();

protected:
	virtual void BeginPlay() override;

	virtual void Tick(float DeltaSeconds) override;

public:
	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;//TakeDamage 오버라이드하여 재정의.

	virtual void End_Hitted() override;//ICharacter의 가상함수 오버라이드.
	void End_Dead() override;//ICharacter의 가상함수 오버라이드.

protected:
	UFUNCTION()
	void RestoreColor();

	virtual void Hitted();//자식(Enemy_AI)에서 재정의 할 수 있도록 virtual을 붙인다.
	void Dead();


protected:
	struct FDamageData
	{
		float Power;
		class ACharacter* Character;
		class AActor* Causer;

		struct FActionDamageEvent* Event; //CWeaponStructure의 FActionDamageEvent
	} Damage; //Damage란 이름으로 저장.

	FVector ImpactPoint_Hit;

public:
	UPROPERTY(EditDefaultsOnly, Category = Combat)
	UAnimMontage* HitReactMontage;

protected:
	void DirectionalHitReact(const FVector& ImpactPoint);

private:
	int ParryingCnt;


//데미지 숫자 구현
public:
	UFUNCTION(BlueprintImplementableEvent)
	void ShowHitNumber(int32 InDamage, FVector InHitLocation);
	
	/** Map to store HitNumber widgets and their hit locations */
	UPROPERTY(VisibleAnywhere, Category = Combat, meta = (AllowPrivateAccess = "true"))
	TMap<UUserWidget*, FVector> HitNumbers;

	UPROPERTY(EditAnywhere, Category = Combat, meta = (AllowPrivateAccess = "true"))
	float HitNumberDestroyTime = 1.0f;

protected:
	UFUNCTION(BlueprintCallable)
	void StoreHitNumber(UUserWidget* InHitNumber, FVector InLocation);

	UFUNCTION()
	void DestroyHitNumber(UUserWidget* InHitNumber);

	void UpdateHitNumbers();
};