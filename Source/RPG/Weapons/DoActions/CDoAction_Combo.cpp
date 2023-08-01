#include "CDoAction_Combo.h"
#include "Global.h"
#include "GameFramework/Character.h"
#include "Components/CStateComponent.h"

UCDoAction_Combo::UCDoAction_Combo()
{
	CHelpers::GetAsset<UAnimMontage>(&InitialLaunchATKMontage, "AnimMontage'/Game/ABP/PP/Montages/Airborne/AM_PP_AirborneATKontage.AM_PP_AirborneATKontage'");
	CHelpers::GetAsset<UAnimMontage>(&InitialLaunchAttackedMontage, "AnimMontage'/Game/Character/Montages/Warp/AM_Airborne_TestMontage.AM_Airborne_TestMontage'");
}

void UCDoAction_Combo::DoAction()
{
	CheckTrue(DoActionDatas.Num() < 1);

	if (bEnable) //bEnable�̶�� Combo ����
	{
		bEnable = false;
		bExist = true;

		return;
	}

	CheckFalse(State->IsIdleMode());

	Super::DoAction(); //ù Ÿ���� �� �Ŀ� Combo�� bEnable�� ȣ��Ǿ�� �Ѵ�. �׷��� �� ���� �ƴ� ���⿡ ��ġ�Ѵ�.//ù Ÿ�� �� IsIdleMode()�� üũ ����� �� �θ��� DoAction���� �� State->ActionMode()�� �����Ѵ�.
	DoActionDatas[Index].DoAction(OwnerCharacter);
}

void UCDoAction_Combo::Begin_DoAction()
{
	Super::Begin_DoAction();
	CheckFalse(bExist);//���� Combo�� ������ �ٷ� End_DoAction���� �̵�.

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
	if (OwnerCharacter)
	{
		OwnerCharacter->PlayAnimMontage(InitialLaunchATKMontage, 1);
	}
}

void UCDoAction_Combo::DoAction_AirCombo()
{
	CheckTrue(DoActionDatas_AirCombo.Num() < 1);
	
	if (bEnable) //bEnable�̶�� Combo ����
	{
		bEnable = false;
		bExist = true;

		return;
	}

	CheckFalse(State->IsIdleMode());


	Super::DoAction_AirCombo();//ù Ÿ���� �� �Ŀ� Combo�� bEnable�� ȣ��Ǿ�� �Ѵ�. �׷��� �� ���� �ƴ� ���⿡ ��ġ�Ѵ�.//ù Ÿ�� �� IsIdleMode()�� üũ ����� �� �θ��� DoAction���� �� State->ActionMode()�� �����Ѵ�.

	DoActionDatas_AirCombo[Index_AirCombo].DoAction_AirCombo(OwnerCharacter);
}

void UCDoAction_Combo::Begin_DoAction_AirCombo()
{
	Super::Begin_DoAction_AirCombo();
	CheckFalse(bExist);//���� Combo�� ������ �ٷ� End_DoAction���� �̵�.

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
	Super::OnAttachmentBeginOverlap(InAttacker, InAttackCauser, InOther);//CDoAction.h�� OnAttachmentBeginOverlap
	CheckNull(InOther);
	CheckTrue(OwnerCharacter == InOther);//2023.07.28 Blade �߰��ϸ鼭 �߰���.with ���߾�.

	for (ACharacter* hitted : Hitted)
		CheckTrue(hitted == InOther);

	Hitted.AddUnique(InOther);

	CheckTrue(HitDatas.Num() - 1 < Index);

	if (false == bInAction_AirCombo)//�Ϲ� ����
	{
		if (InitialLaunchATK)//���� ���� ����
		{
			InitialLaunchATK = false;

			FActionDamageEvent e;
			e.HitData->Montage = InitialLaunchAttackedMontage;
			e.HitData->Power = 5.0f;
			e.HitData->Launch = FVector(50.0f, 0.0f, 2000.0f);

			e.HitData->SendDamage(InAttacker, InAttackCauser, InOther);

			InAttacker->LaunchCharacter(FVector(0, 0, 1200), false, false);//Player ����
		}
		else //���� �Ϲݰ���
		{
			HitDatas[Index].SendDamage(InAttacker, InAttackCauser, InOther);
		}
	}
	else //true == bInAction_AirCombo �����޺� ����
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
