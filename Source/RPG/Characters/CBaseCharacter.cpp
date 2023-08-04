#include "CBaseCharacter.h"
#include "Utilities/CHelpers.h"
#include "Components/CStatusComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/CMontagesComponent.h"
#include "Weapons/CWeaponStructures.h"
#include "Kismet/KismetMathLibrary.h"

ACBaseCharacter::ACBaseCharacter()
{
	PrimaryActorTick.bCanEverTick = true;

	CHelpers::CreateActorComponent<UCWeaponComponent>(this, &Weapon, "Weapon");
	CHelpers::CreateActorComponent<UCMontagesComponent>(this, &Montages, "Montage");
	CHelpers::CreateActorComponent<UCMovementComponent>(this, &Movement, "Movement");
	CHelpers::CreateActorComponent<UCStateComponent>(this, &State, "State");
	CHelpers::CreateActorComponent<UCStatusComponent>(this, &Status, "Status");
}

void ACBaseCharacter::BeginPlay()
{
	Super::BeginPlay();
	
}

float ACBaseCharacter::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator,
	AActor* DamageCauser)
{
	float damage = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);

	Damage.Power = damage;
	Damage.Character = Cast<ACharacter>(EventInstigator->GetPawn());
	Damage.Causer = DamageCauser;
	Damage.Event = (FActionDamageEvent*)&DamageEvent;//���۷����� ĳ�����ϸ� ĳ������ �� �ǰų� ĳ���� �� ����� �� �Դ� ��찡 �߻��� �� �ִ�. //�����ϰ� C��Ÿ�� ĳ�����ϱ� ���� �����ͷ� ĳ�����Ѵ�. 

	State->SetHittedMode();//HittedMode�� ����.

	return damage;
}

void ACBaseCharacter::Hitted()
{
	//Apply Damage ������ ó��
	{   //���� Ÿ�� �� ������ �ʰ� �ϱ����� �ʱ�ȭ ���ش�.
		Status->Damage(Damage.Power);
		Damage.Power = 0;
	}

	//Change Color
	{
		Change_Color(this, FLinearColor::Red);

		FTimerDelegate timerDelegate;
		timerDelegate.BindUFunction(this, "RestoreColor");

		GetWorld()->GetTimerManager().SetTimer(RestoreColor_TimerHandle, timerDelegate, 0.2f, false);//0.2�� ���� �ݺ����� ����.
	}

	//Damage�� CEnemy.h�� FDamageData�̴�.
	if (!!Damage.Event && !!Damage.Event->HitData)
	{
		FHitData* data = Damage.Event->HitData;//FDamageData�� FActionDamageEvent* Event���� HitData

		data->PlayMontage(this);  //��Ÿ�� ���
		data->PlayHitStop(GetWorld());  //HitStop ���
		data->PlaySoundWave(this);//�Ҹ� ���
		data->PlayEffect(GetWorld(), GetActorLocation(), GetActorRotation());//Effect ���

		if (Status->IsDead() == false)
		{
			FVector start = GetActorLocation(); //start=���� ��ġ
			FVector target = Damage.Character->GetActorLocation();//target=�������� �����ϴ� ���
			FVector direction = target - start;//direction=target�� �ٶ󺸴� ����
			direction.Normalize();

			//FVector LaunchedVector = FVector(-direction.X * data->Launch.X, -direction.Y * data->Launch.Y, data->Launch.Z);

			FVector LaunchF = FVector(-direction.X, -direction.Y, 0) * data->Launch.X;
			FVector LaunchR = FVector(0, 0, 0);
			FVector LaunchU = FVector(0, 0, 1) * data->Launch.Z;

			FVector LaunchedVector = LaunchF + LaunchR + LaunchU;

			LaunchCharacter(LaunchedVector, true, true);
			SetActorRotation(UKismetMathLibrary::FindLookAtRotation(start, target));
		}
	}

	//��� ó��
	if (Status->IsDead())
	{
		State->SetDeadMode();//State�� DeadMode�� ������ش�.

		return;
	}

	Damage.Character = nullptr;
	Damage.Causer = nullptr;
	Damage.Event = nullptr;
}

void ACBaseCharacter::End_Hitted()
{
	State->SetIdleMode();
}

void ACBaseCharacter::RestoreColor()
{
	Change_Color(this, OriginColor);//���� ������ �����ش�.

	GetWorld()->GetTimerManager().ClearTimer(RestoreColor_TimerHandle);//RestoreColor_TimerHandle�� �ʱ�ȭ.
}

void ACBaseCharacter::Dead()
{
	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);//Collision ����.

	Montages->PlayDeadMode();
}

void ACBaseCharacter::End_Dead()
{
	Destroy(); //������ �Ҹ�����ش�.
}

///////////////////////////////////////////////////////////////////////////////
