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
#include "Components/ArrowComponent.h"//파쿠르 Arrow 생성을 위한 헤더

//무기 Pickup
#include "Item/CItem.h"
#include "Weapons/CAttachment.h"
#include "Weapons/Attachments/CAttachment_Bow.h"

ACPlayer::ACPlayer()
{
	CHelpers::CreateComponent<USpringArmComponent>(this, &SpringArm, "SpringArm", GetMesh());
	CHelpers::CreateComponent<UCameraComponent>(this, &Camera, "Camera", SpringArm);

	CHelpers::CreateActorComponent<UCWeaponComponent>(this, &Weapon, "Weapon");
	CHelpers::CreateActorComponent<UCMontagesComponent>(this, &Montages, "Montages");
	CHelpers::CreateActorComponent<UCMovementComponent>(this, &Movement, "Movement");
	CHelpers::CreateActorComponent<UCStateComponent>(this, &State, "State");
	CHelpers::CreateActorComponent<UCParkourComponent>(this, &Parkour, "Parkour");
	CHelpers::CreateActorComponent<UCZoomComponent>(this, &Zoom, "Zoom");
	CHelpers::CreateActorComponent<USplineComponent>(this, &ArrowPathSpline, "ArrowPathSpline");

	GetMesh()->SetRelativeLocation(FVector(0, 0, -90));
	GetMesh()->SetRelativeRotation(FRotator(0, -90, 0));

	USkeletalMesh* mesh;
	CHelpers::GetAsset<USkeletalMesh>(&mesh, "SkeletalMesh'/Game/Character/Mesh/SK_Mannequin.SK_Mannequin'");
	GetMesh()->SetSkeletalMesh(mesh);

	TSubclassOf<UCAnimInstance> animInstance;
	CHelpers::GetClass<UCAnimInstance>(&animInstance, "AnimBlueprint'/Game/ABP_Character.ABP_Character_C'");
	GetMesh()->SetAnimClass(animInstance);

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

	State->OnStateTypeChanged.AddDynamic(this, &ACPlayer::OnStateTypeChanged);
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

	PlayerInputComponent->BindAction("Action", EInputEvent::IE_Pressed, Weapon, &UCWeaponComponent::DoAction);

	PlayerInputComponent->BindAction("SubAction", EInputEvent::IE_Pressed, this, &ACPlayer::Click_RightButton);
	PlayerInputComponent->BindAction("SubAction", EInputEvent::IE_Released, Weapon, &UCWeaponComponent::SubAction_Released);

	PlayerInputComponent->BindAction("Equip", EInputEvent::IE_Pressed, this, &ACPlayer::FKeyPressed);
}

void ACPlayer::OnStateTypeChanged(EStateType InPrevType, EStateType InNewType)
{
	switch (InNewType)
	{
		case EStateType::BackStep: BackStep(); break;
	}
}

void ACPlayer::OnAvoid()
{
	CheckFalse(State->IsIdleMode());
	CheckFalse(Movement->CanMove());

	CheckTrue(InputComponent->GetAxisValue("MoveForward") >= 0.0f);//뒷방향을 입력했다면

	State->SetBackStepMode();//State을 BackStepMode로 변경한다.
}

void ACPlayer::BackStep()
{
	Movement->EnableControlRotation();//정면을 바라본 상태로 뒤로 뛰어야하기 때문에 EnableControlRotation으로 만들어준다.

	Montages->PlayBackStepMode();//PlayBackStepMode()를 통해 몽타주 재생.
}

void ACPlayer::End_BackStep()
{
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
	if(Weapon->ItemsArray.Find(Item))
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

void ACPlayer::Landed(const FHitResult& Hit)
{
	Parkour->DoParkour(true);//땅에 떨어졌을때 파쿠르동작 수행
}

void ACPlayer::SetZooming(float InValue)
{
	CheckTrue(Weapon->IsBowMode());//BowMode이면 바로 리턴

	Zoom->SetZoomValue(InValue);//BowMode가 아닌 경우 Zoom In&Out 가능.
}
