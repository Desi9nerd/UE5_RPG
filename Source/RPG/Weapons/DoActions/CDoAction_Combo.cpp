#include "CDoAction_Combo.h"
#include "Global.h"
#include "GameFramework/Character.h"
#include "Components/CStateComponent.h"

void UCDoAction_Combo::DoAction()
{
	CheckTrue(DoActionDatas.Num() < 1);

	if (bEnable) //bEnable이라면 Combo 구간
	{
		bEnable = false;
		bExist = true;

		return;
	}

	CheckFalse(State->IsIdleMode());

	Super::DoAction(); //첫 타격이 들어간 후에 Combo의 bEnable이 호출되어야 한다. 그래서 맨 위가 아닌 여기에 위치한다.//첫 타격 시 IsIdleMode()를 체크 통과한 후 부모의 DoAction으로 들어가 State->ActionMode()로 변경한다.
	DoActionDatas[Index].DoAction(OwnerCharacter);
}

void UCDoAction_Combo::Begin_DoAction()
{
	Super::Begin_DoAction();
	CheckFalse(bExist);//다음 Combo가 없으면 바로 End_DoAction으로 이동.

	bExist = false;
	DoActionDatas[++Index].DoAction(OwnerCharacter);
}

void UCDoAction_Combo::End_DoAction()
{
	Super::End_DoAction();

	Index = 0;
}

void UCDoAction_Combo::AirborneInitATK()
{
	CheckFalse(State->IsIdleMode());

	InitialLaunchATK = true;

	DoActionDatas_AirborneInitATK[0].DoAction(OwnerCharacter);
}

void UCDoAction_Combo::DoAction_AirCombo()
{
	CheckTrue(DoActionDatas_AirCombo.Num() < 1);
	
	if (bEnable) //bEnable이라면 Combo 구간
	{
		bEnable = false;
		bExist = true;

		return;
	}

	CheckFalse(State->IsIdleMode());


	Super::DoAction_AirCombo();//첫 타격이 들어간 후에 Combo의 bEnable이 호출되어야 한다. 그래서 맨 위가 아닌 여기에 위치한다.//첫 타격 시 IsIdleMode()를 체크 통과한 후 부모의 DoAction으로 들어가 State->ActionMode()로 변경한다.

	DoActionDatas_AirCombo[Index_AirCombo].DoAction_AirCombo(OwnerCharacter);
}

void UCDoAction_Combo::Begin_DoAction_AirCombo()
{
	Super::Begin_DoAction_AirCombo();
	CheckFalse(bExist);//다음 Combo가 없으면 바로 End_DoAction으로 이동.

	bExist = false;
	DoActionDatas_AirCombo[++Index_AirCombo].DoAction_AirCombo(OwnerCharacter);
}

void UCDoAction_Combo::End_DoAction_AirCombo()
{
	Super::End_DoAction_AirCombo();

	Index_AirCombo = 0;
}

void UCDoAction_Combo::OnAttachmentBeginOverlap(ACharacter* InAttacker, AActor* InAttackCauser, ACharacter* InOther)
{
	Super::OnAttachmentBeginOverlap(InAttacker, InAttackCauser, InOther);//CDoAction.h의 OnAttachmentBeginOverlap
	CheckNull(InOther);
	CheckTrue(OwnerCharacter == InOther);//2023.07.28 Blade 추가하면서 추가함.with 현중씨.

	for (ACharacter* hitted : Hitted)
		CheckTrue(hitted == InOther);

	Hitted.AddUnique(InOther);

	CheckTrue(HitDatas.Num() - 1 < Index);

	if (false == bInAction_AirCombo)//일반 공격
	{
		if (InitialLaunchATK)//공중 띄우기 공격
		{
			InitialLaunchATK = false;
						
			InAttacker->LaunchCharacter(FVector(0, 0, 1200), false, false);//Player 띄우기

			HitDatas_AirborneInitATK[0].SendDamage(InAttacker, InAttackCauser, InOther);
		}
		else //지상 일반공격
		{
			HitDatas[Index].SendDamage(InAttacker, InAttackCauser, InOther);
		}
	}
	else //true == bInAction_AirCombo 공중콤보 공격
	{
		HitDatas_AirCombo[Index_AirCombo].SendDamage(InAttacker, InAttackCauser, InOther);
	}
}

void UCDoAction_Combo::OnAttachmentEndCollision()
{
	Super::OnAttachmentEndCollision();

	float angle = -2.0f;
	ACharacter* candidate = nullptr;

	for (ACharacter* hitted : Hitted)
	{
		FVector direction = hitted->GetActorLocation() - OwnerCharacter->GetActorLocation();
		direction = direction.GetSafeNormal2D();


		FVector forward = FQuat(OwnerCharacter->GetActorRotation()).GetForwardVector();

		float dot = FVector::DotProduct(direction, forward);
		if (dot >= angle)
		{
			angle = dot;
			candidate = hitted;
		}
	}

	if (!!candidate)
	{
		FRotator rotator = UKismetMathLibrary::FindLookAtRotation(OwnerCharacter->GetActorLocation(), candidate->GetActorLocation());
		FRotator target = FRotator(0, rotator.Yaw, 0);

		AController* controller = OwnerCharacter->GetController<AController>();
		controller->SetControlRotation(target);
	}

	Hitted.Empty();
}
