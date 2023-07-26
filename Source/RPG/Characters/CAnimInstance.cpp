#include "CAnimInstance.h"
#include "Global.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Weapons/CSubAction.h"
#include "Components/CMovementComponent.h"
#include "Parkour/CParkourComponent.h"
#include "Components/CFeetComponent.h"

void UCAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();

	OwnerCharacter = Cast<ACharacter>(TryGetPawnOwner());
	CheckNull(OwnerCharacter);

	CharacterMovement = OwnerCharacter->GetCharacterMovement();

	Movement = CHelpers::GetComponent<UCMovementComponent>(OwnerCharacter);

	Weapon = CHelpers::GetComponent<UCWeaponComponent>(OwnerCharacter);
	if (!!Weapon)
		Weapon->OnWeaponTypeChange.AddDynamic(this, &UCAnimInstance::OnWeaponTypeChanged);
}

void UCAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);
	CheckNull(OwnerCharacter);

	//속도, 공중유무
	{
		Speed = OwnerCharacter->GetVelocity().Size2D();
		IsFalling = CharacterMovement->IsFalling();
		bFalling = OwnerCharacter->GetCharacterMovement()->IsFalling();
	}

	//회전
	{
		FRotator rotator = OwnerCharacter->GetVelocity().ToOrientationRotator();
		FRotator rotator2 = OwnerCharacter->GetControlRotation();
		FRotator delta = UKismetMathLibrary::NormalizedDeltaRotator(rotator, rotator2);
		PrevRotation = UKismetMathLibrary::RInterpTo(PrevRotation, delta, DeltaSeconds, 25);
		Direction = PrevRotation.Yaw;

		Pitch = UKismetMathLibrary::FInterpTo(Pitch, OwnerCharacter->GetBaseAimRotation().Pitch, DeltaSeconds, 25);
	}

	
	CheckNull(Weapon);//무기가 있는지 확인

	//파쿠르, Feet IK
	UCParkourComponent* parkour = CHelpers::GetComponent<UCParkourComponent>(OwnerCharacter);
	UCFeetComponent* feet = CHelpers::GetComponent<UCFeetComponent>(OwnerCharacter);

	bFeet = false;

	if (Weapon->IsUnarmedMode())
	{
		if (!!parkour && !!feet)
		{
			bFeet = parkour->IsExecuting() == false;//EParkourType::Max라면 false==false가 되어 bFeet은 true, EParkourType::Max가 아닌 상황이라면 true==false가 되어 bFeet은 false.
			FeetData = feet->GetData();//FFeetData를 넣어준다.
		}
		else if (!!feet)
		{
			bFeet = true;
			FeetData = feet->GetData();
		}
	}

	//활 조준
	if (!!Weapon->GetSubAction())
	{
		bBow_Aiming = true;
		bBow_Aiming &= WeaponType == EWeaponType::Bow;
		bBow_Aiming &= Weapon->GetSubAction()->GetInAction();
	}

	//더블 점프, MovementComponent의 JumpCount를 받아오는 함수 호출.
	JumpCnt = Movement->DoubleJumpCount();
}

void UCAnimInstance::OnWeaponTypeChanged(EWeaponType InPrevType, EWeaponType InNewType)
{
	WeaponType = InNewType;
}