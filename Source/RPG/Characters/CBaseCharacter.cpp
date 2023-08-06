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
	Damage.Event = (FActionDamageEvent*)&DamageEvent;//���۷����� ĳ�����ϸ� ĳ������ �� �ǰų� ĳ���� �� ����� �� �Դ� ��찡 �߻��� �� �ִ�. //�����ϰ� C��Ÿ�� ĳ�����ϱ� ���� �����ͷ� ĳ�����Ѵ�.

	
	ACAttachment* CustomDamageEvent = (ACAttachment*)&DamageEvent;
	ImpactPoint_Hit = CustomDamageEvent->HitResult_CAttachment.ImpactPoint;// Access the impact point

	

	State->SetHittedMode();//HittedMode�� ����.

	return damage;
}

void ACBaseCharacter::Hitted()
{
	//Block ��
	if (State->IsParryingMode())
	{
		LaunchCharacter(GetActorForwardVector() * -200.0f, false, false);
	
	
		return;
	}

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




		//�̰� ����
		FVector HitExactLocation = ImpactPoint_Hit;//���� üũ!! �Ƹ� ���� �ʿ�!!







		if(data->Montage)//FHitData�� �Ҵ��� ��Ÿ�ְ� �ִٸ�
		{
			data->PlayMontage(this);  //��Ÿ�� ���
		}
		else//�Ҵ��� ��Ÿ�ְ� ���ٸ� �⺻ HitReactMontage ���
		{
			DirectionalHitReact(HitExactLocation);
		}

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
			//SetActorRotation(UKismetMathLibrary::FindLookAtRotation(start, target));
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

void ACBaseCharacter::DirectionalHitReact(const FVector& ImpactPoint)
{
	const FVector Forward = GetActorForwardVector();
	const FVector ImpactLowered(ImpactPoint.X, ImpactPoint.Y, GetActorLocation().Z);//����뿡�ο찡 �ٴڰ� �����̵ǰ� ����� ���� ImpactPoint.Z�� ��ſ� z���� Enemy��ġ z������ ������ش�.
	const FVector ToHit = (ImpactLowered - GetActorLocation()).GetSafeNormal();//(�浹����-Enemy��ġ)�� Normalize�Ѵ�.

	/* ���� */
	//����. Forward * ToHit = |Forward||ToHit| * cos(theta)
	// |Forward| = 1, |ToHit| = 1 �̹Ƿ� Forward * ToHit = cos(theta)
	const double CosTheta = FVector::DotProduct(Forward, ToHit);
	//Theta�� ���ϱ� ���� arc-cosine�� �Ѵ�.
	double Theta = FMath::Acos(CosTheta);
	//Theta�� ����(radian) ���̴�. ���� ���� degree�� �ٲپ��ش�.
	Theta = FMath::RadiansToDegrees(Theta);



	///
	//if (GEngine) //����� �޽���. �浹 �� Theta �� ���. //���Ŀ� ����
	//{
	//	GEngine->AddOnScreenDebugMessage(1, 5.0f, FColor::Green, FString::Printf(TEXT("Theta: %f"),Theta));
	//}
	//UKismetSystemLibrary::DrawDebugArrow(this, GetActorLocation(), GetActorLocation() + Forward * 100.0f,10.0f, //FLinearColor::Red, 5.0f);
	//	UKismetSystemLibrary::DrawDebugArrow(this, GetActorLocation(), GetActorLocation() + ToHit * 100.0f, 10.0f, FLinearColor::Green, 5.0f);
	///



	/* ���� */
	//Forward�� ToHit�� ������ ���Ѵ�.
	////�������Ͱ� �Ʒ��� ���ϰ� ������, Theta���� ����(-)�̴�.//�𸮾��� �޼���ǥ��
	const FVector CrossProduct = FVector::CrossProduct(Forward, ToHit);
	if (CrossProduct.Z < 0)
	{
		Theta *= -1.0f; //
	}

	FName Section("FromBack");//�� ���� �ǰ�

	if (Theta >= -45.0f && Theta < 45.0f)
	{
		Section = FName("FromFront");//�� ���� �ǰ�
	}
	else if (Theta >= -135.0f && Theta < -45.0f)
	{
		Section = FName("FromLeft");//���� ���� �ǰ�
	}
	else if (Theta >= 45.0f && Theta < 135.0f)
	{
		Section = FName("FromRight");//������ ���� �ǰ�
	}
	
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	if (AnimInstance && HitReactMontage)
	{
		AnimInstance->Montage_Play(HitReactMontage);
		AnimInstance->Montage_JumpToSection(Section, HitReactMontage);
	}

	//������. ���Ŀ� ����
	//UKismetSystemLibrary::DrawDebugArrow(this, GetActorLocation(), GetActorLocation() + Forward + CrossProduct * 150.0f, 10.0f, FColor::Blue, 5.0f);

}

void ACBaseCharacter::End_Dead()
{
	Destroy(); //������ �Ҹ�����ش�.
}

///////////////////////////////////////////////////////////////////////////////
