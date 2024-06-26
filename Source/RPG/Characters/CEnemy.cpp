#include "CEnemy.h"
#include "Global.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/SkeletalMeshComponent.h"

ACEnemy::ACEnemy()
{
	GetMesh()->SetRelativeLocation(FVector(0, 0, -90));
	GetMesh()->SetRelativeRotation(FRotator(0, -90, 0));
	
	GetCharacterMovement()->RotationRate = FRotator(0, 720, 0);
}

void ACEnemy::BeginPlay()
{
	Super::BeginPlay();

	Create_DynamicMaterial(this);//IICharacter의 함수 사용. 색이 적용되는 객체를 this(여기서는 Enemy)로 설정.
	Change_Color(this, OriginColor);//색상 할당.

	State->OnStateTypeChanged.AddDynamic(this, &ACEnemy::OnStateTypeChanged);
}

void ACEnemy::OnStateTypeChanged(EStateType InPrevType, EStateType InNewType)
{
	Super::OnStateTypeChanged(InPrevType, InNewType);

	switch (InNewType)
	{
		case EStateType::Hitted: Hitted(); break;//실제처리는 아래의 Hitted()에서 일어난다.
		case EStateType::Dead:   Dead(); break;//실제처리는 아래의 Dead()에서 일어난다.
		default: break;
	}
}

void ACEnemy::Hitted()
{
	Super::Hitted();
}

void ACEnemy::End_Hitted()
{
	Super::End_Hitted();
}