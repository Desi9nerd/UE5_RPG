#include "CMovementComponent.h"
#include "Global.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/CMontagesComponent.h"

UCMovementComponent::UCMovementComponent()
{

}

void UCMovementComponent::BeginPlay()
{
	Super::BeginPlay();

	OwnerCharacter = Cast<ACharacter>(GetOwner());
	JumpCount = 0;
}

void UCMovementComponent::SetSpeed(ESpeedType InType)
{
	OwnerCharacter->GetCharacterMovement()->MaxWalkSpeed = Speed[(int32)InType];
}

void UCMovementComponent::OnJump()
{
	if (false == OwnerCharacter->GetCharacterMovement()->IsFalling() &&
		JumpCount == 0)
	{
		OwnerCharacter->Jump();
		JumpCount++;
	}
	else if (JumpCount == 1)
	{
		OwnerCharacter->Jump();//Jump�� ���Ͽ��� �Ǵ� ������? ABP_Character���� JumpCount�� Ȱ���Ͽ� ���� ������ �ϰ� State�� ����� �ٸ� �ִϸ��̼��� ���´�.
		JumpCount++;
		OwnerCharacter->LaunchCharacter(FVector(0.0f, 0.0f, 500.0f), false, true);
	}
}

void UCMovementComponent::SetJumpCountZero()
{
	JumpCount = 0;
}

void UCMovementComponent::OnStopJumping()
{
	OwnerCharacter->StopJumping();	
}

void UCMovementComponent::OnSprintFast()
{
	SetSpeed(ESpeedType::SprintFast);
	DisableControlRotation();
}

void UCMovementComponent::OnSprint()
{
	SetSpeed(ESpeedType::Sprint);
	EnableControlRotation();
}

void UCMovementComponent::OnRun()
{
	SetSpeed(ESpeedType::Run);
}

void UCMovementComponent::OnWalk()
{
	SetSpeed(ESpeedType::Walk);
}

void UCMovementComponent::EnableControlRotation()
{
	OwnerCharacter->bUseControllerRotationYaw = true;
	OwnerCharacter->GetCharacterMovement()->bOrientRotationToMovement = false;
}

void UCMovementComponent::DisableControlRotation()
{
	OwnerCharacter->bUseControllerRotationYaw = false;
	OwnerCharacter->GetCharacterMovement()->bOrientRotationToMovement = true;
}

void UCMovementComponent::OnMoveForward(float InAxis)
{
	CheckFalse(bCanMove);

	FRotator rotator = FRotator(0, OwnerCharacter->GetControlRotation().Yaw, 0);
	FVector direction = FQuat(rotator).GetForwardVector();

	if (bTopViewCamera)//(Warp)TopView ī�޶� ����Ѵٸ�
		direction = FVector::XAxisVector;//��������� XAxisVector�� �������� ���.

	OwnerCharacter->AddMovementInput(direction, InAxis);
}

void UCMovementComponent::OnMoveRight(float InAxis)
{
	CheckFalse(bCanMove);

	FRotator rotator = FRotator(0, OwnerCharacter->GetControlRotation().Yaw, 0);
	FVector direction = FQuat(rotator).GetRightVector();

	if (bTopViewCamera)//(Warp)TopView ī�޶� ����Ѵٸ�
		direction = FVector::YAxisVector;//��������� YAxisVector�� �������� ���.

	OwnerCharacter->AddMovementInput(direction, InAxis);
}

void UCMovementComponent::OnHorizontalLook(float InAxis)
{
	CheckTrue(bFixedCamera);

	OwnerCharacter->AddControllerYawInput(InAxis * HorizontalLook * GetWorld()->GetDeltaSeconds());
}

void UCMovementComponent::OnVerticalLook(float InAxis)
{
	CheckTrue(bFixedCamera);

	OwnerCharacter->AddControllerPitchInput(InAxis * VerticalLook * GetWorld()->GetDeltaSeconds());
}