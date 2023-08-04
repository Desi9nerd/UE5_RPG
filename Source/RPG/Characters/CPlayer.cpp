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
#include "Components/ArrowComponent.h"//파쿠르 Arrow 생성을 위한 헤더

//무기 Pickup
#include "Item/CItem.h"
#include "Weapons/CAttachment.h"
#include "Weapons/Attachments/CAttachment_Bow.h"

ACPlayer::ACPlayer()
{
	CHelpers::CreateComponent<USpringArmComponent>(this, &SpringArm, "SpringArm", GetMesh());
	CHelpers::CreateComponent<UCameraComponent>(this, &Camera, "Camera", SpringArm);

	//CHelpers::CreateActorComponent<UCWeaponComponent>(this, &Weapon, "Weapon");
	//CHelpers::CreateActorComponent<UCMontagesComponent>(this, &Montages, "Montages");
	//CHelpers::CreateActorComponent<UCMovementComponent>(this, &Movement, "Movement");
	//CHelpers::CreateActorComponent<UCStateComponent>(this, &State, "State");
	CHelpers::CreateActorComponent<UCParkourComponent>(this, &Parkour, "Parkour");
	CHelpers::CreateActorComponent<UCZoomComponent>(this, &Zoom, "Zoom");
	CHelpers::CreateActorComponent<USplineComponent>(this, &ArrowPathSpline, "ArrowPathSpline");

	GetMesh()->SetRelativeLocation(FVector(0, 0, -90));
	GetMesh()->SetRelativeRotation(FRotator(0, -90, 0));
	
	SpringArm->SetRelativeLocation(FVector(0, 0, 140));
	SpringArm->SetRelativeRotation(FRotator(0, 90, 0));
	SpringArm->TargetArmLength = 200;
	SpringArm->bDoCollisionTest = false;
	SpringArm->bUsePawnControlRotation = true;
	SpringArm->bEnableCameraLag = true;

	GetCharacterMovement()->RotationRate = FRotator(0, 720, 0);

	/** 파쿠르 */
	CHelpers::CreateComponent<USceneComponent>(this, &ArrowGroup, "ArrowGroup", GetCapsuleComponent());//ArrowGroup을 생성하고 GetCapsuleComponent 아래로 붙여준다.
	//파쿠르에 활용한 Arrows들을 생성한다. 
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
	/** 파쿠르 */
}

void ACPlayer::BeginPlay()
{
	Super::BeginPlay();

	Movement->OnRun(); //Movement의 기본을 Run으로 설정
	Movement->DisableControlRotation();//Movement의 기본을 DisableControlRotation으로 설정

	CheckNull(State);
	
	State->OnStateTypeChanged.AddDynamic(this, &ACPlayer::OnStateTypeChanged);
	
	PlayerController = Cast<APlayerController>(GetController());//PlayerController 캐스팅
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

	PlayerInputComponent->BindAction("Avoid", EInputEvent::IE_Pressed, this, &ACPlayer::OnAvoid);

	//PlayerInputComponent->BindAxis("Zoom", Zoom, &UCZoomComponent::SetZoomValue);//Value로 연동되게하면 Bow 모드 시 문제가 될 수 있다.
	PlayerInputComponent->BindAxis("Zoom", this, &ACPlayer::SetZooming);//Bow 모드가 아닐 때만 사용하려고 SetZooming으로 설정하여 만들었다.

	PlayerInputComponent->BindAction("Fist", EInputEvent::IE_Pressed, Weapon, &UCWeaponComponent::SetFistMode);
	PlayerInputComponent->BindAction("Sword", EInputEvent::IE_Pressed, Weapon, &UCWeaponComponent::SetSwordMode);
	PlayerInputComponent->BindAction("Hammer", EInputEvent::IE_Pressed, Weapon, &UCWeaponComponent::SetHammerMode);
	PlayerInputComponent->BindAction("Warp", EInputEvent::IE_Pressed, Weapon, &UCWeaponComponent::SetWarpMode);
	PlayerInputComponent->BindAction("Around", EInputEvent::IE_Pressed, Weapon, &UCWeaponComponent::SetAroundMode);
	PlayerInputComponent->BindAction("Bow", EInputEvent::IE_Pressed, Weapon, &UCWeaponComponent::SetBowMode);
	PlayerInputComponent->BindAction("Blade", EInputEvent::IE_Pressed, Weapon, &UCWeaponComponent::SetBladeMode);

	PlayerInputComponent->BindAction("Action", EInputEvent::IE_Pressed, Weapon, &UCWeaponComponent::DoAction);

	PlayerInputComponent->BindAction("SubAction", EInputEvent::IE_Pressed, this, &ACPlayer::Click_RightButton);
	PlayerInputComponent->BindAction("SubAction", EInputEvent::IE_Released, Weapon, &UCWeaponComponent::SubAction_Released);

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
		case EStateType::Hitted: Hitted(); break;//실제처리는 아래의 Hitted()에서 일/어/난다.
		case EStateType::Dead:   Dead(); break;//실제처리는 아래의 Dead()에서 일어난다.
		default: break;
	}
}

void ACPlayer::OnAvoid()
{
	CheckNull(State);
	CheckFalse(State->IsIdleMode());
	CheckFalse(Movement->CanMove());

	CheckTrue(InputComponent->GetAxisValue("MoveForward") == 0.0f && InputComponent->GetAxisValue("MoveRight") == 0.0f);


	State->SetDodgeMode();//State을 DodgeMode로 변경한다.
}

void ACPlayer::Dodge()
{
	DisableInput(PlayerController);//Dodge가 시작되면 키 입력이 안 되게 만들어준다.

	Movement->EnableControlRotation();//정면을 바라본 상태로 뒤로 뛰어야하기 때문에 EnableControlRotation으로 만들어준다.

	Montages->PlayDodgeMode();//PlayDodgeMode()를 통해 몽타주 재생.
}

void ACPlayer::End_Dodge()
{
	EnableInput(PlayerController);//Dodge가 끝날때 키 입력이 다시 되게 되돌려준다.

	Movement->DisableControlRotation();//Backstep이 끝나면 원래대로 돌려준다.

	State->SetIdleMode();//Idle상태로 돌려줌.
}

void ACPlayer::FKeyPressed()
{
	CLog::Print(L"F Key Pressed!");

	ACAttachment* OverlappingWeapon = Cast<ACAttachment>(OverlappingItem);
	if (!!OverlappingWeapon)
	{
		Weapon->ItemsArray.Add(OverlappingWeapon);
		OverlappingWeapon->AttachTo(OverlappingWeapon->GetHolsterSocketName());

		OverlappingWeapon = nullptr;
		OverlappingItem = nullptr;
	}

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

void ACPlayer::Click_RightButton()
{
	if (Weapon->IsUnarmedMode())
	{
		Parkour->DoParkour();

		return;
	}

	Weapon->SubAction_Pressed();
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
	Parkour->DoParkour(true);//땅에 떨어졌을때 파쿠르동작 수행

	Movement->SetJumpCountZero();
}

void ACPlayer::SetZooming(float InValue)
{
	CheckTrue(Weapon->IsBowMode());//BowMode이면 바로 리턴

	Zoom->SetZoomValue(InValue);//BowMode가 아닌 경우 Zoom In&Out 가능.
}