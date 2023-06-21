#pragma once

#include "CoreMinimal.h"
#include "ICharacter.h"
#include "Components/CStateComponent.h"
#include "GameFramework/Character.h"
#include "Characters/CBaseCharacter.h"
#include "CEnemy.generated.h"

UCLASS()
class RPG_API ACEnemy
	: public ACBaseCharacter, public IICharacter //다중상속
{
	GENERATED_BODY()

private:
	UPROPERTY(EditAnywhere, Category = "Color")
		FLinearColor OriginColor = FLinearColor::White;//색상 설정.

public:
	ACEnemy();

public:
	float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;//TakeDamage 오버라이드하여 재정의.

protected:
	virtual void BeginPlay() override;

private:
	UFUNCTION()
		void OnStateTypeChanged(EStateType InPrevType, EStateType InNewType);

private:
	void Hitted();

public:
	void End_Hitted() override;//ICharacter의 가상함수 오버라이드.

private:
	UFUNCTION()
	void RestoreColor();

private:
	void Dead();

public:
	void End_Dead() override;//ICharacter의 가상함수 오버라이드.

private:
	struct FDamageData
	{
		float Power;
		class ACharacter* Character;
		class AActor* Causer;

		struct FActionDamageEvent* Event; //CWeaponStructure의 FActionDamageEvent
	} Damage; //Damage란 이름으로 저장.

private:
	FTimerHandle RestoreColor_TimerHandle;//변한 색이 일정시간 후에 되돌아오도록 시간을 기록하는 변수.

};