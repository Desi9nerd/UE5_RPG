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

	//기본(마우스 왼쪽클릭) 공격
	void DoAction() override;
	void Begin_DoAction() override;
	void End_DoAction() override;

	//공중으로 띄우는 공격
	void AirborneInitATK() override;//CDoAction.h의 함수 재정의. 
	//공중 콤보
	void DoAction_AirCombo() override;
	void Begin_DoAction_AirCombo() override;
	void End_DoAction_AirCombo() override;

	//패링
	void Parrying_Start() override;
	void Parrying_End() override;

	void OnAttachmentBeginOverlap(class ACharacter* InAttacker, AActor*InAttackCauser, class ACharacter* InOther) override;
	void OnAttachmentEndCollision() override;
	
	bool bExist;

private:
	int32 Index;
	bool bEnable;
	
	TArray<ACharacter*> Hitted;

	//공중콤보
	int32 Index_AirCombo;
	bool InitialLaunchATK;

	//패링 몽타주
	UAnimMontage* Parrying_Start_Montage;
	UAnimMontage* Parrying_End_Montage;
};
