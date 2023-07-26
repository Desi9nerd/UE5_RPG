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

	//�ӵ�, ��������
	{
		Speed = OwnerCharacter->GetVelocity().Size2D();
		IsFalling = CharacterMovement->IsFalling();
		bFalling = OwnerCharacter->GetCharacterMovement()->IsFalling();
	}

	//ȸ��
	{
		FRotator rotator = OwnerCharacter->GetVelocity().ToOrientationRotator();
		FRotator rotator2 = OwnerCharacter->GetControlRotation();
		FRotator delta = UKismetMathLibrary::NormalizedDeltaRotator(rotator, rotator2);
		PrevRotation = UKismetMathLibrary::RInterpTo(PrevRotation, delta, DeltaSeconds, 25);
		Direction = PrevRotation.Yaw;

		Pitch = UKismetMathLibrary::FInterpTo(Pitch, OwnerCharacter->GetBaseAimRotation().Pitch, DeltaSeconds, 25);
	}

	
	CheckNull(Weapon);//���Ⱑ �ִ��� Ȯ��

	//����, Feet IK
	UCParkourComponent* parkour = CHelpers::GetComponent<UCParkourComponent>(OwnerCharacter);
	UCFeetComponent* feet = CHelpers::GetComponent<UCFeetComponent>(OwnerCharacter);

	bFeet = false;

	if (Weapon->IsUnarmedMode())
	{
		if (!!parkour && !!feet)
		{
			bFeet = parkour->IsExecuting() == false;//EParkourType::Max��� false==false�� �Ǿ� bFeet�� true, EParkourType::Max�� �ƴ� ��Ȳ�̶�� true==false�� �Ǿ� bFeet�� false.
			FeetData = feet->GetData();//FFeetData�� �־��ش�.
		}
		else if (!!feet)
		{
			bFeet = true;
			FeetData = feet->GetData();
		}
	}

	//Ȱ ����
	if (!!Weapon->GetSubAction())
	{
		bBow_Aiming = true;
		bBow_Aiming &= WeaponType == EWeaponType::Bow;
		bBow_Aiming &= Weapon->GetSubAction()->GetInAction();
	}

	//���� ����, MovementComponent�� JumpCount�� �޾ƿ��� �Լ� ȣ��.
	JumpCnt = Movement->DoubleJumpCount();
}

void UCAnimInstance::OnWeaponTypeChanged(EWeaponType InPrevType, EWeaponType InNewType)
{
	WeaponType = InNewType;
}