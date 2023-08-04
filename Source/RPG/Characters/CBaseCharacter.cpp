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
	Damage.Event = (FActionDamageEvent*)&DamageEvent;//레퍼런스로 캐스팅하면 캐스팅이 안 되거나 캐스팅 후 상속이 안 먹는 경우가 발생할 수 있다. //안전하게 C스타일 캐스팅하기 위해 포인터로 캐스팅한다. 

	State->SetHittedMode();//HittedMode로 변경.

	return damage;
}

void ACBaseCharacter::Hitted()
{
	//Apply Damage 데미지 처리
	{   //다음 타격 때 꼬이지 않게 하기위해 초기화 해준다.
		Status->Damage(Damage.Power);
		Damage.Power = 0;
	}

	//Change Color
	{
		Change_Color(this, FLinearColor::Red);

		FTimerDelegate timerDelegate;
		timerDelegate.BindUFunction(this, "RestoreColor");

		GetWorld()->GetTimerManager().SetTimer(RestoreColor_TimerHandle, timerDelegate, 0.2f, false);//0.2초 동안 반복없이 실행.
	}

	//Damage는 CEnemy.h의 FDamageData이다.
	if (!!Damage.Event && !!Damage.Event->HitData)
	{
		FHitData* data = Damage.Event->HitData;//FDamageData의 FActionDamageEvent* Event내의 HitData

		data->PlayMontage(this);  //몽타주 재생
		data->PlayHitStop(GetWorld());  //HitStop 재생
		data->PlaySoundWave(this);//소리 재생
		data->PlayEffect(GetWorld(), GetActorLocation(), GetActorRotation());//Effect 재생

		if (Status->IsDead() == false)
		{
			FVector start = GetActorLocation(); //start=현재 위치
			FVector target = Damage.Character->GetActorLocation();//target=데미지를 전달하는 대상
			FVector direction = target - start;//direction=target을 바라보는 방향
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

	//사망 처리
	if (Status->IsDead())
	{
		State->SetDeadMode();//State을 DeadMode로 만들어준다.

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
	Change_Color(this, OriginColor);//원래 색으로 돌려준다.

	GetWorld()->GetTimerManager().ClearTimer(RestoreColor_TimerHandle);//RestoreColor_TimerHandle를 초기화.
}

void ACBaseCharacter::Dead()
{
	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);//Collision 꺼줌.

	Montages->PlayDeadMode();
}

void ACBaseCharacter::End_Dead()
{
	Destroy(); //죽으면 소멸시켜준다.
}

///////////////////////////////////////////////////////////////////////////////
