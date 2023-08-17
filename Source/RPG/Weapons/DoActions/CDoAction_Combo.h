#pragma once

#include "CoreMinimal.h"
#include "Weapons/CDoAction.h"
#include "CDoAction_Combo.generated.h"

UCLASS()
class RPG_API UCDoAction_Combo : public UCDoAction
{
	GENERATED_BODY()

private:
	UCDoAction_Combo();
		
public:
	FORCEINLINE void EnableCombo() { bEnable = true; }
	FORCEINLINE void DisableCombo() { bEnable = false; }

public:
	void DoAction() override;
	void Begin_DoAction() override;
	void End_DoAction() override;

	//�������� ���� ����
	void AirborneInitATK() override;//CDoAction.h�� �Լ� ������. 
	//���� �޺�
	void DoAction_AirCombo() override;
	void Begin_DoAction_AirCombo() override;
	void End_DoAction_AirCombo() override;

	//�и�
	void Parrying_Start() override;
	void Parrying_End() override;

public:
	void OnAttachmentBeginOverlap(class ACharacter* InAttacker, AActor*InAttackCauser, class ACharacter* InOther) override;
	void OnAttachmentEndCollision() override;
	
private:
	int32 Index;

	bool bEnable;

public:
	bool bExist;

private:
	TArray<class ACharacter*> Hitted;


//�����޺�
private:
	int32 Index_AirCombo;
	bool InitialLaunchATK;

//�и�
private:
	UAnimMontage* Parrying_Start_Montage;
	UAnimMontage* Parrying_End_Montage;
};
