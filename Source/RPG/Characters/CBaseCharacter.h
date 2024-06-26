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
class UCMovementComponent;
class UCWeaponComponent;
class UCMontagesComponent;
class UCStatusComponent;
class UCTargetComponent;
class UTextRenderComponent;

UCLASS()
class RPG_API ACBaseCharacter
	: public ACharacter, public IGenericTeamAgentInterface, public IIHit, public IICharacter //���߻��
{
	GENERATED_BODY()

public:
	ACBaseCharacter();
	FGenericTeamId GetGenericTeamId() const override { return FGenericTeamId(TeamID); }
	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;//TakeDamage �������̵��Ͽ� ������.
	FName DirectionalHitReactSection(const FVector& ImpactPoint);

	//********************************************************
	//** IICharacter
	virtual void End_Hitted() override;
	void End_Dead() override;
	//********************************************************

	UFUNCTION()
	virtual void OnStateTypeChanged(EStateType InPrevType, EStateType InNewType) { };

	UFUNCTION(BlueprintImplementableEvent)
	void ShowHitNumber(int32 InDamage, FVector InHitLocation);//������ ���� ����

	UPROPERTY(EditDefaultsOnly, Category = Combat)
	UAnimMontage* HitReactMontage;

protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaSeconds) override;

	virtual void Hitted();//�ڽ�(Enemy_AI)���� ������ �� �� �ֵ��� virtual�� ���δ�.
	void Dead();
	void DirectionalHitReact(const FVector& ImpactPoint);

	UFUNCTION()
	void RestoreColor();

	UFUNCTION(BlueprintCallable)
	void StoreHitNumber(UUserWidget* InHitNumber, FVector InLocation);
	UFUNCTION()
	void DestroyHitNumber(UUserWidget* InHitNumber);

	void UpdateHitNumbers();


	UPROPERTY(EditAnywhere, Category = "Team")
	uint8 TeamID;// TeamID�� 0~255������ ���������ϴ�. 255���� �߸��̴�. ID ������ �Ʊ��̰� ID�� �ٸ��� ���̴�.

	UPROPERTY(VisibleAnywhere)
	UCWeaponComponent* Weapon;

	UPROPERTY(VisibleAnywhere)
	UCMontagesComponent* Montages;

	UPROPERTY(VisibleAnywhere)
	UCMovementComponent* Movement;

	UPROPERTY(VisibleAnywhere)
	UCStateComponent* State;

	UPROPERTY(VisibleAnywhere)
	UCStatusComponent* Status;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UCTargetComponent* Target;

	UPROPERTY(VisibleAnywhere)
	UTextRenderComponent* TextRender_State;
	UPROPERTY(VisibleAnywhere)
	UTextRenderComponent* TextRender_Weapon;

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

private:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Combat, meta = (AllowPrivateAccess = "true"))
	TMap<UUserWidget*, FVector> HitNumbers; // HitNumber ����, Hit location

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Combat, meta = (AllowPrivateAccess = "true"))
	float HitNumberDestroyTime = 1.0f;

	int ParryingCnt;
};