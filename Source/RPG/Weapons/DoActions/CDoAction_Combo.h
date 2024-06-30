#pragma once
#include "CoreMinimal.h"
#include "Weapons/CDoAction.h"
#include "CDoAction_Combo.generated.h"

UCLASS()
class RPG_API UCDoAction_Combo : public UCDoAction
{
	GENERATED_BODY()
		
public:
	FORCEINLINE void EnableCombo() { bEnable = true; }
	FORCEINLINE void DisableCombo() { bEnable = false; }

	FORCEINLINE bool GetbEnable() { return bEnable; }
	FORCEINLINE bool GetbExist() { return bExist; }

	UCDoAction_Combo();

	//�⺻(���콺 ����Ŭ��) ����
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

	void OnAttachmentBeginOverlap(class ACharacter* InAttacker, AActor*InAttackCauser, class ACharacter* InOther) override;
	void OnAttachmentEndCollision() override;
	
	bool bExist;

private:
	int32 Index;
	bool bEnable;
	
	TArray<ACharacter*> Hitted;

	//�����޺�
	int32 Index_AirCombo;
	bool InitialLaunchATK;

	//�и� ��Ÿ��
	UAnimMontage* Parrying_Start_Montage;
	UAnimMontage* Parrying_End_Montage;
};
