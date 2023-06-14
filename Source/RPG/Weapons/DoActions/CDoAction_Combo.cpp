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

void UCDoAction_Combo::OnAttachmentBeginOverlap(ACharacter* InAttacker, AActor* InAttackCauser, ACharacter* InOther)
{
	Super::OnAttachmentBeginOverlap(InAttacker, InAttackCauser, InOther);//CDoAction.h의 OnAttachmentBeginOverlap
	CheckNull(InOther);

	//TakeDamage 체크용 디버그
	//CLog::Log(InOther->GetName());
	//FActionDamageEvent e;
	//e.HitData = &HitDatas[0];
	//InOther->TakeDamage(20, FDamageEvent(), InAttacker->GetController(), InAttackCauser);//데미지 적용.
	//InOther->TakeDamage(e.HitData->Power, e, InAttacker->GetController(), InAttackCauser);//업캐스팅. 업캐스팅은 항상 성립한다.

	for (ACharacter* hitted : Hitted)
		CheckTrue(hitted == InOther);

	Hitted.AddUnique(InOther);

	CheckTrue(HitDatas.Num() - 1 < Index);

	HitDatas[Index].SendDamage(InAttacker, InAttackCauser, InOther);
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

