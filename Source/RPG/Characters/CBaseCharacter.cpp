#include "CBaseCharacter.h"
#include "Utilities/CHelpers.h"
#include "Components/CStatusComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/CMontagesComponent.h"
#include "Components/CWeaponComponent.h"
#include "Components/TextRenderComponent.h"
#include "Weapons/CWeaponStructures.h"
#include "Weapons/CAttachment.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/KismetTextLibrary.h"
//Widget
#include "HUD/CHitNumber.h"
#include "Blueprint/UserWidget.h"
#include "Kismet/GameplayStatics.h"
#include "CEnemy.h"
#include "CPlayer.h"

ACBaseCharacter::ACBaseCharacter()
{
	PrimaryActorTick.bCanEverTick = true;

	CHelpers::CreateActorComponent<UCWeaponComponent>(this, &Weapon, "Weapon");
	CHelpers::CreateActorComponent<UCMontagesComponent>(this, &Montages, "Montage");
	CHelpers::CreateActorComponent<UCMovementComponent>(this, &Movement, "Movement");
	CHelpers::CreateActorComponent<UCStateComponent>(this, &State, "State");
	CHelpers::CreateActorComponent<UCStatusComponent>(this, &Status, "Status");
	CHelpers::CreateActorComponent<UTextRenderComponent>(this, &TextRender_State, "TextRender_State");
	CHelpers::CreateActorComponent<UTextRenderComponent>(this, &TextRender_Weapon, "TextRender_Weapon");

	CHelpers::CreateActorComponent<UAnimMontage>(this, &HitReactMontage, "HitReactMontage");

	TextRender_State->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);
	TextRender_State->SetRelativeLocation(FVector(0, 0, 110));
	TextRender_State->SetRelativeRotation(FRotator(0, 0, 0));

	TextRender_Weapon->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);
	TextRender_Weapon->SetRelativeLocation(FVector(0, 0, 90));
	TextRender_Weapon->SetRelativeRotation(FRotator(0, 0, 0));
}

void ACBaseCharacter::BeginPlay()
{
	Super::BeginPlay();

	State->OnStateTypeChanged.AddDynamic(this, &ACBaseCharacter::OnStateTypeChanged);

	//������, �Ӹ� ���� ���� State, Weapon Type ����
	TextRender_State->SetVisibility(false);
	TextRender_Weapon->SetVisibility(false);
}

void ACBaseCharacter::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	//������, �Ӹ� ���� ���� State, Weapon Type ����
	FString name = StaticEnum<EStateType>()->GetNameStringByValue((int32)State->GetStateType());
	TextRender_State->SetText(FText::FromString(name));

	FString WeaponTypeName = StaticEnum<EWeaponType>()->GetNameStringByValue((int32)Weapon->GetWeaponType());
	TextRender_Weapon->SetText(UKismetTextLibrary::Conv_StringToText(WeaponTypeName));

	//HitNumber ������Ʈ
	UpdateHitNumbers();
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

	//**
	//HitNumber �����ϱ�

	//ACEnemy* HitEnemy = Cast<ACEnemy>(EventInstigator->GetCharacter());
	//if (HitEnemy)
	//	HitEnemy->ShowHitNumber(Damage.Power, ImpactPoint_Hit);
	ACPlayer* PlayerTemp = Cast<ACPlayer>(EventInstigator->GetCharacter());
	if (PlayerTemp)
		PlayerTemp->ShowHitNumber(Damage.Power, ImpactPoint_Hit);

	//**	

	//** */ Hit ���� �Ǵ�
	//Hit�� Impact Point�� ����ؼ� ���� �������� �Ǵ��Ѵ�. Parrying ���� ���θ� �����ϱ� ����
	const FVector Forward = GetActorForwardVector();
	const FVector ImpactLowered(ImpactPoint_Hit.X, ImpactPoint_Hit.Y, GetActorLocation().Z);//
	const FVector ToHit = (ImpactLowered - GetActorLocation()).GetSafeNormal();
	//����
	const double CosTheta = FVector::DotProduct(Forward, ToHit);
	double Theta = FMath::Acos(CosTheta);
	Theta = FMath::RadiansToDegrees(Theta);
	//����
	const FVector CrossProduct = FVector::CrossProduct(Forward, ToHit);
	if (CrossProduct.Z < 0)
		Theta *= -1.0f;

	bool bFrontHitted = false;
	if (Theta >= -60.0f && Theta < 60.0f)//���� ���� 120�� �϶��� �и� ��� 
		bFrontHitted = true;
	//** */


	//Parrying ���� + �� ���� �ǰ� + �и����� 5 �̸��� ��
	if (State->IsParryingMode() && bFrontHitted == true && ACBaseCharacter::ParryingCnt < 5)
	{
		LaunchCharacter(GetActorForwardVector() * -200.0f, false, false);
		ParryingCnt++;//�и�ī��Ʈ ����

		return NULL;
	}
	ParryingCnt = 0;//���� ���� �и��� �ǵ��� �ʱ�ȭ


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
		FVector HitExactLocation = ImpactPoint_Hit;

		//HitData ����
		FHitData* data = Damage.Event->HitData;//FDamageData�� FActionDamageEvent* Event���� HitData

		if(data->CharacterCnM.Num() > 0)//FHitData�� �Ҵ��� ��Ÿ�ְ� �ִٸ� data->CharacterClass != nullptr &&
		{
			data->PlayMontage(this);//��Ÿ�� ���
		}
		//else//�Ҵ��� ��Ÿ�ְ� ���ٸ� �⺻ HitReactMontage ���
		//{
		//	DirectionalHitReact(HitExactLocation);
		//}

		data->PlayHitStop(GetWorld());  //HitStop ���
		data->PlaySoundWave(this);//�Ҹ� ���
		data->PlayEffect(GetWorld(), GetActorLocation(), GetActorRotation());//Effect ���

		if (Status->IsDead() == false)
		{
			FVector start = GetActorLocation(); //start=���� ��ġ
			FVector target = Damage.Character->GetActorLocation();//target=�������� �����ϴ� ���
			FVector direction = target - start;//direction=target�� �ٶ󺸴� ����
			direction.Normalize();
		
			//FVector LaunchedVector = FVector(-direction.X * data->Launch.X, -direction.Y * data->Launch.Y,data-//>Launch.Z);
		
			FVector LaunchF = FVector(-direction.X, -direction.Y, 0) * data->Launch.X * 10.0f;
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

	//FName Section("Default");//���� ������ Default
	FName Section("FromBack");//���� ������ Default

	//if (Theta <= -45.0f && Theta > 45.0f)
	//{
	//	Section = FName("FromBack");//�� ���� �ǰ�
	//}
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


void ACBaseCharacter::StoreHitNumber(UUserWidget* InHitNumber, FVector InLocation)
{
	HitNumbers.Add(InHitNumber, InLocation);

	FTimerHandle HitNumberTimer;
	FTimerDelegate HitNumberDelegate;
	HitNumberDelegate.BindUFunction(this, FName("DestroyHitNumber"), InHitNumber);

	GetWorld()->GetTimerManager().SetTimer(HitNumberTimer, HitNumberDelegate, HitNumberDestroyTime, false);

}

void ACBaseCharacter::DestroyHitNumber(UUserWidget* InHitNumber)
{
	HitNumbers.Remove(InHitNumber);
	InHitNumber->RemoveFromParent();//#include "Blueprint/UserWidget.h"�ʿ�
}

void ACBaseCharacter::UpdateHitNumbers()
{
	for (auto& HitPair : HitNumbers)
	{
		//UUserWidgetŸ���� HitNumber������ �����ϰ� Key������ �ʱ�ȭ
		UUserWidget* HitNumber{ HitPair.Key };
		//FVectorŸ���� Location������ �����ϰ� Value������ �ʱ�ȭ
		const FVector Location{ HitPair.Value };
		FVector2D ScreenPosition;

		//SreenPosition ��ġ���� Location ��ġ�� ����Ͽ� �־��ش�.
		//ScreenPosition�� FVector2D�̰� Location�� FVector�̱� ������ �Ʒ��� ���� ��ȯ�� �ʿ��ϴ�.
		UGameplayStatics::ProjectWorldToScreen(GetWorld()->GetFirstPlayerController(), Location, ScreenPosition);

		//HitNumber�� ScreenPosition ��ġ�� ������Ʈ.
		HitNumber->SetPositionInViewport(ScreenPosition);
	}	
}
