#include "CBaseCharacter.h"
#include "Utilities/CHelpers.h"
#include "Components/CStatusComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/CMontagesComponent.h"
#include "Weapons/CWeaponStructures.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Weapons/CAttachment.h"

ACBaseCharacter::ACBaseCharacter()
{
	PrimaryActorTick.bCanEverTick = true;

	CHelpers::CreateActorComponent<UCWeaponComponent>(this, &Weapon, "Weapon");
	CHelpers::CreateActorComponent<UCMontagesComponent>(this, &Montages, "Montage");
	CHelpers::CreateActorComponent<UCMovementComponent>(this, &Movement, "Movement");
	CHelpers::CreateActorComponent<UCStateComponent>(this, &State, "State");
	CHelpers::CreateActorComponent<UCStatusComponent>(this, &Status, "Status");

	CHelpers::CreateActorComponent<UAnimMontage>(this, &HitReactMontage, "HitReactMontage");
}

void ACBaseCharacter::BeginPlay()
{
	Super::BeginPlay();
	
}

float ACBaseCharacter::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	float damage = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);

	Damage.Power = damage;
	Damage.Character = Cast<ACharacter>(EventInstigator->GetPawn());
	Damage.Causer = DamageCauser;
	Damage.Event = (FActionDamageEvent*)&DamageEvent;//레퍼런스로 캐스팅하면 캐스팅이 안 되거나 캐스팅 후 상속이 안 먹는 경우가 발생할 수 있다. //안전하게 C스타일 캐스팅하기 위해 포인터로 캐스팅한다.

	
	ACAttachment* CustomDamageEvent = (ACAttachment*)&DamageEvent;
	ImpactPoint_Hit = CustomDamageEvent->HitResult_CAttachment.ImpactPoint;// Access the impact point

	

	State->SetHittedMode();//HittedMode로 변경.

	return damage;
}

void ACBaseCharacter::Hitted()
{
	//Block 시
	if (State->IsParryingMode())
	{
		LaunchCharacter(GetActorForwardVector() * -200.0f, false, false);
	
	
		return;
	}

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




		//이거 수정
		FVector HitExactLocation = ImpactPoint_Hit;//여기 체크!! 아마 수정 필요!!







		if(data->Montage)//FHitData에 할당한 몽타주가 있다면
		{
			data->PlayMontage(this);  //몽타주 재생
		}
		else//할당한 몽타주가 없다면 기본 HitReactMontage 재생
		{
			DirectionalHitReact(HitExactLocation);
		}

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
			//SetActorRotation(UKismetMathLibrary::FindLookAtRotation(start, target));
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

void ACBaseCharacter::DirectionalHitReact(const FVector& ImpactPoint)
{
	const FVector Forward = GetActorForwardVector();
	const FVector ImpactLowered(ImpactPoint.X, ImpactPoint.Y, GetActorLocation().Z);//디버깅에로우가 바닥과 평행이되게 만들기 위해 ImpactPoint.Z값 대신에 z값을 Enemy위치 z값으로 만들어준다.
	const FVector ToHit = (ImpactLowered - GetActorLocation()).GetSafeNormal();//(충돌지점-Enemy위치)를 Normalize한다.

	/* 내적 */
	//내적. Forward * ToHit = |Forward||ToHit| * cos(theta)
	// |Forward| = 1, |ToHit| = 1 이므로 Forward * ToHit = cos(theta)
	const double CosTheta = FVector::DotProduct(Forward, ToHit);
	//Theta를 구하기 위해 arc-cosine을 한다.
	double Theta = FMath::Acos(CosTheta);
	//Theta는 라디안(radian) 값이다. 라디안 값을 degree로 바꾸어준다.
	Theta = FMath::RadiansToDegrees(Theta);



	///
	//if (GEngine) //디버깅 메시지. 충돌 시 Theta 값 출력. //추후에 삭제
	//{
	//	GEngine->AddOnScreenDebugMessage(1, 5.0f, FColor::Green, FString::Printf(TEXT("Theta: %f"),Theta));
	//}
	//UKismetSystemLibrary::DrawDebugArrow(this, GetActorLocation(), GetActorLocation() + Forward * 100.0f,10.0f, //FLinearColor::Red, 5.0f);
	//	UKismetSystemLibrary::DrawDebugArrow(this, GetActorLocation(), GetActorLocation() + ToHit * 100.0f, 10.0f, FLinearColor::Green, 5.0f);
	///



	/* 외적 */
	//Forward와 ToHit의 외적을 구한다.
	////외적벡터가 아래를 향하고 있으면, Theta값은 음수(-)이다.//언리얼은 왼손좌표계
	const FVector CrossProduct = FVector::CrossProduct(Forward, ToHit);
	if (CrossProduct.Z < 0)
	{
		Theta *= -1.0f; //
	}

	FName Section("FromBack");//뒷 방향 피격

	if (Theta >= -45.0f && Theta < 45.0f)
	{
		Section = FName("FromFront");//앞 방향 피격
	}
	else if (Theta >= -135.0f && Theta < -45.0f)
	{
		Section = FName("FromLeft");//왼쪽 방향 피격
	}
	else if (Theta >= 45.0f && Theta < 135.0f)
	{
		Section = FName("FromRight");//오른쪽 방향 피격
	}
	
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	if (AnimInstance && HitReactMontage)
	{
		AnimInstance->Montage_Play(HitReactMontage);
		AnimInstance->Montage_JumpToSection(Section, HitReactMontage);
	}

	//디버깅용. 추후에 삭제
	//UKismetSystemLibrary::DrawDebugArrow(this, GetActorLocation(), GetActorLocation() + Forward + CrossProduct * 150.0f, 10.0f, FColor::Blue, 5.0f);

}

void ACBaseCharacter::End_Dead()
{
	Destroy(); //죽으면 소멸시켜준다.
}

///////////////////////////////////////////////////////////////////////////////
