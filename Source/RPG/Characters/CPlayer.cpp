#include "Characters/CPlayer.h"
#include "Global.h"
#include "CAnimInstance.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Camera/CameraComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/InputComponent.h"
#include "Components/CMontagesComponent.h"
#include "Components/CMovementComponent.h"
#include "Components/CWeaponComponent.h"
#include "Components/CZoomComponent.h"
#include "Components/CStateComponent.h"
#include "Components/ArrowComponent.h"//���� Arrow ������ ���� ���

//���� Pickup
#include "Item/CItem.h"
#include "Weapons/CAttachment.h"
#include "Weapons/Attachments/CAttachment_Bow.h"
//HUD
#include "Components/CStatusComponent.h"
#include "Components/CTargetComponent.h"
#include "Components/WidgetComponent.h"
#include "HUD/CCharacterHUD.h"
#include "HUD/CPlayerOverlay.h"
#include "HUD/CPromptText.h"

ACPlayer::ACPlayer()
{
	CHelpers::CreateComponent<USpringArmComponent>(this, &SpringArm, "SpringArm", GetMesh());
	CHelpers::CreateComponent<UCameraComponent>(this, &Camera, "Camera", SpringArm);
	CHelpers::CreateActorComponent<UCParkourComponent>(this, &Parkour, "Parkour");
	CHelpers::CreateActorComponent<UCZoomComponent>(this, &Zoom, "Zoom");
	CHelpers::CreateActorComponent<UCTargetComponent>(this, &Target, "Target");

	GetMesh()->SetRelativeLocation(FVector(0, 0, -90));
	GetMesh()->SetRelativeRotation(FRotator(0, -90, 0));
	
	SpringArm->SetRelativeLocation(FVector(0, 0, 140));
	SpringArm->SetRelativeRotation(FRotator(0, 90, 0));
	SpringArm->TargetArmLength = 200;
	SpringArm->bDoCollisionTest = false;
	SpringArm->bUsePawnControlRotation = true;
	SpringArm->bEnableCameraLag = true;

	GetCharacterMovement()->RotationRate = FRotator(0, 720, 0);

	/** ���� */
	CHelpers::CreateComponent<USceneComponent>(this, &ArrowGroup, "ArrowGroup", GetCapsuleComponent());//ArrowGroup�� �����ϰ� GetCapsuleComponent �Ʒ��� �ٿ��ش�.
	//������ Ȱ���� Arrows���� �����Ѵ�. 
	for (int32 i = 0; i < (int32)EParkourArrowType::Max; i++)
	{
		FString name = StaticEnum<EParkourArrowType>()->GetNameStringByIndex(i);
		CHelpers::CreateComponent<UArrowComponent>(this, &Arrows[i], FName(name), ArrowGroup);

		switch ((EParkourArrowType)i)
		{
		case EParkourArrowType::Center:
			Arrows[i]->ArrowColor = FColor::Red;
			break;

		case EParkourArrowType::Ceil:
			Arrows[i]->ArrowColor = FColor::Green;
			Arrows[i]->SetRelativeLocation(FVector(0, 0, 100));
			break;

		case EParkourArrowType::Floor:
			Arrows[i]->ArrowColor = FColor::Blue;
			Arrows[i]->SetRelativeLocation(FVector(0, 0, -80));
			break;

		case EParkourArrowType::Left:
			Arrows[i]->ArrowColor = FColor::Magenta;
			Arrows[i]->SetRelativeLocation(FVector(0, -30, 0));
			break;

		case EParkourArrowType::Right:
			Arrows[i]->ArrowColor = FColor::Magenta;
			Arrows[i]->SetRelativeLocation(FVector(0, 30, 0));
			break;

		case EParkourArrowType::Land:
			Arrows[i]->ArrowColor = FColor::Yellow;
			Arrows[i]->SetRelativeLocation(FVector(200, 0, 100));
			Arrows[i]->SetRelativeRotation(FRotator(-90, 0, 0));
			break;
		}
	}
	/** ���� */
}

void ACPlayer::BeginPlay()
{
	Super::BeginPlay();

	Movement->OnRun();//Movement�� �⺻�� Run���� ����
	Movement->DisableControlRotation();//Movement�� �⺻�� DisableControlRotation���� ����

	CheckNull(State);
	
	State->OnStateTypeChanged.AddDynamic(this, &ACPlayer::OnStateTypeChanged);
	
	PlayerController = Cast<APlayerController>(GetController());//PlayerController ĳ����
	InitializePlayerOverlay();//PlayerHUD ����
}

void ACPlayer::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	if (PlayerOverlay && Status)
	{
		Status->RegenStamina(DeltaSeconds);
		PlayerOverlay->SetStaminaBarPercent(Status->GetStaminaPercent());
	}
}

void ACPlayer::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis("MoveForward", Movement, &UCMovementComponent::OnMoveForward);
	PlayerInputComponent->BindAxis("MoveRight", Movement, &UCMovementComponent::OnMoveRight);
	PlayerInputComponent->BindAxis("VerticalLook", Movement, &UCMovementComponent::OnVerticalLook);
	PlayerInputComponent->BindAxis("HorizontalLook", Movement, &UCMovementComponent::OnHorizontalLook);

	PlayerInputComponent->BindAction("Jump", EInputEvent::IE_Pressed, Movement, &UCMovementComponent::OnJump);
	PlayerInputComponent->BindAction("Jump", EInputEvent::IE_Released, Movement, &UCMovementComponent::OnStopJumping);

	PlayerInputComponent->BindAction("Sprint", EInputEvent::IE_Pressed, Movement, &UCMovementComponent::OnSprint);
	PlayerInputComponent->BindAction("Sprint", EInputEvent::IE_Released, Movement, &UCMovementComponent::OnRun);
	PlayerInputComponent->BindAction("SprintFast", EInputEvent::IE_Pressed, Movement, &UCMovementComponent::OnSprintFast);
	PlayerInputComponent->BindAction("SprintFast", EInputEvent::IE_Released, Movement, &UCMovementComponent::OnSprint);

	PlayerInputComponent->BindAction("Avoid", EInputEvent::IE_Pressed, this, &ACPlayer::OnAvoid);

	//PlayerInputComponent->BindAxis("Zoom", Zoom, &UCZoomComponent::SetZoomValue);//Value�� �����ǰ��ϸ� Bow ��� �� ������ �� �� �ִ�.
	PlayerInputComponent->BindAxis("Zoom", this, &ACPlayer::SetZooming);//Bow ��尡 �ƴ� ���� ����Ϸ��� SetZooming���� �����Ͽ� �������.

	PlayerInputComponent->BindAction("Fist", EInputEvent::IE_Pressed, Weapon, &UCWeaponComponent::SetFistMode);
	PlayerInputComponent->BindAction("Sword", EInputEvent::IE_Pressed, Weapon, &UCWeaponComponent::SetSwordMode);
	PlayerInputComponent->BindAction("Hammer", EInputEvent::IE_Pressed, Weapon, &UCWeaponComponent::SetHammerMode);
	PlayerInputComponent->BindAction("Warp", EInputEvent::IE_Pressed, Weapon, &UCWeaponComponent::SetWarpMode);
	PlayerInputComponent->BindAction("Around", EInputEvent::IE_Pressed, Weapon, &UCWeaponComponent::SetAroundMode);
	PlayerInputComponent->BindAction("Bow", EInputEvent::IE_Pressed, Weapon, &UCWeaponComponent::SetBowMode);
	PlayerInputComponent->BindAction("Blade", EInputEvent::IE_Pressed, Weapon, &UCWeaponComponent::SetBladeMode);

	PlayerInputComponent->BindAction("Action", EInputEvent::IE_Pressed, Weapon, &UCWeaponComponent::DoAction);

	PlayerInputComponent->BindAction("SubAction", EInputEvent::IE_Pressed, this, &ACPlayer::OnRightButton);
	PlayerInputComponent->BindAction("SubAction", EInputEvent::IE_Released, this, &ACPlayer::OffRightButton);

	PlayerInputComponent->BindAction("MiddleClick", EInputEvent::IE_Pressed, this, &ACPlayer::MiddleMouse_Pressed);
	PlayerInputComponent->BindAction("MiddleClick", EInputEvent::IE_Released, this, &ACPlayer::MiddleMouse_Released);

	PlayerInputComponent->BindAction("Airborne", EInputEvent::IE_Pressed, Weapon, &UCWeaponComponent::AirborneInitATK);

	PlayerInputComponent->BindAction("Equip", EInputEvent::IE_Pressed, this, &ACPlayer::FKeyPressed);
}

void ACPlayer::OnStateTypeChanged(EStateType InPrevType, EStateType InNewType)
{
	switch (InNewType)
	{
		case EStateType::Dodge:	 Dodge(); break;
		case EStateType::Hitted: Hitted(); break;//����ó���� �Ʒ��� Hitted()���� ��/��/����.
		case EStateType::Dead:   Dead(); break;//����ó���� �Ʒ��� Dead()���� �Ͼ��.
		default: break;
	}
}

void ACPlayer::OnAvoid()
{
	CheckNull(State);
	CheckFalse(State->IsIdleMode());
	CheckFalse(Movement->CanMove());

	CheckTrue(InputComponent->GetAxisValue("MoveForward") == 0.0f && InputComponent->GetAxisValue("MoveRight") == 0.0f);

	CheckNull(Status);

	if (Status->GetStamina() < Status->GetDodgeCost())
	{
		//CLog::Log(FString("Low Stamina"));

		ACCharacterHUD* PlayerHUD = Cast<ACCharacterHUD>(PlayerController->GetHUD());
		PlayerHUD->OnStaminaPromptText();

		return;//���� Stamina�� Dodge�� �����ϴµ� �Ҹ�Ǵ� Stamina���� �۴ٸ� ����.
	}
	
	if (PlayerOverlay)
	{
		Status->UseStamina(Status->GetDodgeCost());//Dodge ���� �� Stamina �Ҹ�
		PlayerOverlay->SetStaminaBarPercent(Status->GetStaminaPercent());//StaminaBar ������Ʈ
	}

	State->SetDodgeMode();//State�� DodgeMode�� �����Ѵ�.
}

void ACPlayer::Dodge()
{
	DisableInput(PlayerController);//Dodge�� ���۵Ǹ� Ű �Է��� �� �ǰ� ������ش�.

	//GetMesh()->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	Movement->EnableControlRotation();//������ �ٶ� ���·� �ڷ� �پ���ϱ� ������ EnableControlRotation���� ������ش�.

	Montages->PlayDodgeMode();//PlayDodgeMode()�� ���� ��Ÿ�� ���.
}

void ACPlayer::End_Dodge()
{
	EnableInput(PlayerController);//Dodge�� ������ Ű �Է��� �ٽ� �ǰ� �ǵ����ش�.

	Movement->DisableControlRotation();//Dodge�� ������ ������� �����ش�.

	State->SetIdleMode();//Idle���·� ������.
}

void ACPlayer::FKeyPressed()
{
	CLog::Print(L"F Key Pressed!");

	//ACAttachment* OverlappingWeapon = Cast<ACAttachment>(OverlappingItem);
	//if (!!OverlappingWeapon)
	//{
	//	Weapon->ItemsArray.Add(OverlappingWeapon);
	//	OverlappingWeapon->AttachTo(OverlappingWeapon->GetHolsterSocketName());
	//
	//
	//	OverlappingWeapon = nullptr;
	//	OverlappingItem = nullptr;
	//}

	//else
	//{
	//	if (CanDisarm())
	//	{
	//		Disarm();
	//	}
	//	else if (CanArm())
	//	{
	//		Arm();
	//	}
	//}
}

void ACPlayer::SetOverlappingItem(ACItem* Item)
{
	if (Weapon->ItemsArray.Find(Item))
	{
		OverlappingItem = Item;
	}
}

void ACPlayer::OnRightButton()
{
	if (Weapon->IsUnarmedMode())
	{
		Parkour->DoParkour();

		return;
	}

	if (Weapon->IsBowMode())
	{
		//BowMode(Ȱ ����)�� �� ��Ŭ���� ������ ������ Zoom����� ���ش�.
		Zoom->SetComponentTickEnabled(false);
	}

	Weapon->SubAction_Pressed();
}

void ACPlayer::OffRightButton()
{
	if (Weapon->IsBowMode())
	{
		//BowMode(Ȱ ����)�� �� ��Ŭ���� ���� Zoom����� ���ش�.
		Zoom->SetComponentTickEnabled(true);
	}

	Weapon->SubAction_Released();
}

void ACPlayer::MiddleMouse_Pressed()
{
	if (Weapon->IsBladeMode())
	{
		Weapon->Parrying_Pressed();

		return;
	}
}

void ACPlayer::MiddleMouse_Released()
{
	if (Weapon->IsBladeMode())
	{
		Weapon->Parrying_Released();

		return;
	}
}

void ACPlayer::Landed(const FHitResult& Hit)
{
	Parkour->DoParkour(true);//���� ���������� �������� ����

	Movement->SetJumpCountZero();
}

void ACPlayer::SetZooming(float InValue)
{
	CheckTrue(Weapon->IsBowMode());//BowMode�̸� �ٷ� ����

	Zoom->SetZoomValue(InValue);//BowMode�� �ƴ� ��� Zoom In&Out ����.
}


 
//////////////////////////////////////////////////////////////
  
 
 
float ACPlayer::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	SetHUDHealth();
	
	//**
	//HitNumber �����ϱ�
	//ACEnemy* HitEnemy = Cast<ACEnemy>(DamageEvent.);
	//HitEnemy->ShowHitNumber(Damage.Power, ImpactPoint_Hit);


	return Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);
}

void ACPlayer::InitializePlayerOverlay()
{
	if (!!PlayerController)
	{
		ACCharacterHUD* PlayerHUD = Cast<ACCharacterHUD>(PlayerController->GetHUD());
		if (!!PlayerHUD)
		{
			PlayerOverlay = PlayerHUD->GetPlayerOverlay();
			if (PlayerOverlay && Status)
			{
				PlayerOverlay->SetHealthBarPercent(Status->GetHealthPercent());
				PlayerOverlay->SetStaminaBarPercent(Status->GetStaminaPercent());//���� �� Stamina��
				PlayerOverlay->SetGold(0);
				PlayerOverlay->SetSouls(0);
			}
		}
	}
}

void ACPlayer::SetHUDHealth()
{
	if (PlayerOverlay && Status)
	{
		PlayerOverlay->SetHealthBarPercent(Status->GetHealthPercent());
	}
}
