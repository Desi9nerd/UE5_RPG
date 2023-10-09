#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "ICharacter.h"
#include "Interfaces/IHit.h"
#include "GenericTeamAgentInterface.h"
#include "Characters/ECharacterTypes.h"
#include "Components/CStateComponent.h"
#include "CBaseCharacter.generated.h"

class UAnimMontage;

UCLASS()
class RPG_API ACBaseCharacter
	: public ACharacter, public IGenericTeamAgentInterface, public IIHit, public IICharacter //���߻��
{
	GENERATED_BODY()
		
protected:
	UPROPERTY(EditAnywhere, Category = "Team")
	uint8 TeamID;// TeamID�� 0~255������ ���������ϴ�. 255���� �߸��̴�. ID ������ �Ʊ��̰� ID�� �ٸ��� ���̴�.

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

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UCTargetComponent* Target;

	UPROPERTY(VisibleAnywhere)
	class UTextRenderComponent* TextRender_State;
	UPROPERTY(VisibleAnywhere)
	class UTextRenderComponent* TextRender_Weapon;

	//���� ���� �� ������ ���� ����
	//UPROPERTY(VisibleAnywhere, Category = Weapon)
	//ACWeapon* EquippedWeapon;

	UPROPERTY(EditAnywhere, Category = "Color")
	FLinearColor OriginColor = FLinearColor::White;//���� ����.

	FTimerHandle RestoreColor_TimerHandle;//���� ���� �����ð� �Ŀ� �ǵ��ƿ����� �ð��� ����ϴ� ����.

	struct FDamageData
	{
		float Power;
		class ACharacter* Character;
		class AActor* Causer;

		struct FActionDamageEvent* Event; //CWeaponStructure�� FActionDamageEvent
	} Damage; //Damage�� �̸����� ����.

	FVector ImpactPoint_Hit;

public:
	ACBaseCharacter();
	FGenericTeamId GetGenericTeamId() const override { return FGenericTeamId(TeamID); }

	UFUNCTION()
	virtual void OnStateTypeChanged(EStateType InPrevType, EStateType InNewType) { };

	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;//TakeDamage �������̵��Ͽ� ������.

	virtual void End_Hitted() override;//ICharacter�� �����Լ� �������̵�.
	void End_Dead() override;//ICharacter�� �����Լ� �������̵�.

protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaSeconds) override;
	
	virtual void Hitted();//�ڽ�(Enemy_AI)���� ������ �� �� �ֵ��� virtual�� ���δ�.
	void Dead();

	UFUNCTION()
	void RestoreColor();

	void DirectionalHitReact(const FVector& ImpactPoint);

public:
	UPROPERTY(EditDefaultsOnly, Category = Combat)
	UAnimMontage* HitReactMontage;

	FName DirectionalHitReactSection(const FVector& ImpactPoint);
	
private:
	int ParryingCnt;


//������ ���� ����
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