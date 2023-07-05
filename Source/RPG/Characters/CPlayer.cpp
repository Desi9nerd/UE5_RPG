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

ACPlayer::ACPlayer()
{
	CHelpers::CreateComponent<USpringArmComponent>(this, &SpringArm, "SpringArm", GetMesh());
	CHelpers::CreateComponent<UCameraComponent>(this, &Camera, "Camera", SpringArm);

	CHelpers::CreateActorComponent<UCWeaponComponent>(this, &Weapon, "Weapon");
	CHelpers::CreateActorComponent<UCMontagesComponent>(this, &Montages, "Montages");
	CHelpers::CreateActorComponent<UCMovementComponent>(this, &Movement, "Movement");
	CHelpers::CreateActorComponent<UCStateComponent>(this, &State, "State");

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

	PlayerInputComponent->BindAction("Sprint", EInputEvent::IE_Pressed, Movement, &UCMovementComponent::OnSprint);
	PlayerInputComponent->BindAction("Sprint", EInputEvent::IE_Released, Movement, &UCMovementComponent::OnRun);

	PlayerInputComponent->BindAction("Avoid", EInputEvent::IE_Pressed, this, &ACPlayer::OnAvoid);

	PlayerInputComponent->BindAction("Fist", EInputEvent::IE_Pressed, Weapon, &UCWeaponComponent::SetFistMode);
	PlayerInputComponent->BindAction("Sword", EInputEvent::IE_Pressed, Weapon, &UCWeaponComponent::SetSwordMode);
	PlayerInputComponent->BindAction("Hammer", EInputEvent::IE_Pressed, Weapon, &UCWeaponComponent::SetHammerMode);
	PlayerInputComponent->BindAction("Warp", EInputEvent::IE_Pressed, Weapon, &UCWeaponComponent::SetWarpMode);
	PlayerInputComponent->BindAction("Around", EInputEvent::IE_Pressed, Weapon, &UCWeaponComponent::SetAroundMode);
	PlayerInputComponent->BindAction("Bow", EInputEvent::IE_Pressed, Weapon, &UCWeaponComponent::SetBowMode);

	PlayerInputComponent->BindAction("Action", EInputEvent::IE_Pressed, Weapon, &UCWeaponComponent::DoAction);

	PlayerInputComponent->BindAction("SubAction", EInputEvent::IE_Pressed, Weapon,&UCWeaponComponent::SubAction_Pressed);
	PlayerInputComponent->BindAction("SubAction", EInputEvent::IE_Released, Weapon, &UCWeaponComponent::SubAction_Released);
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