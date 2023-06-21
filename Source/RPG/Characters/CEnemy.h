#pragma once

#include "CoreMinimal.h"
#include "ICharacter.h"
#include "Components/CStateComponent.h"
#include "GameFramework/Character.h"
#include "Characters/CBaseCharacter.h"
#include "CEnemy.generated.h"

UCLASS()
class RPG_API ACEnemy
	: public ACBaseCharacter, public IICharacter //���߻��
{
	GENERATED_BODY()

private:
	UPROPERTY(EditAnywhere, Category = "Color")
		FLinearColor OriginColor = FLinearColor::White;//���� ����.

public:
	ACEnemy();

public:
	float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;//TakeDamage �������̵��Ͽ� ������.

protected:
	virtual void BeginPlay() override;

private:
	UFUNCTION()
		void OnStateTypeChanged(EStateType InPrevType, EStateType InNewType);

private:
	void Hitted();

public:
	void End_Hitted() override;//ICharacter�� �����Լ� �������̵�.

private:
	UFUNCTION()
	void RestoreColor();

private:
	void Dead();

public:
	void End_Dead() override;//ICharacter�� �����Լ� �������̵�.

private:
	struct FDamageData
	{
		float Power;
		class ACharacter* Character;
		class AActor* Causer;

		struct FActionDamageEvent* Event; //CWeaponStructure�� FActionDamageEvent
	} Damage; //Damage�� �̸����� ����.

private:
	FTimerHandle RestoreColor_TimerHandle;//���� ���� �����ð� �Ŀ� �ǵ��ƿ����� �ð��� ����ϴ� ����.

};