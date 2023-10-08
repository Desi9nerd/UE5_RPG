#include "CEnemy.h"
#include "Global.h"
#include "CAnimInstance.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/CMovementComponent.h"

ACEnemy::ACEnemy()
{
	GetMesh()->SetRelativeLocation(FVector(0, 0, -90));
	GetMesh()->SetRelativeRotation(FRotator(0, -90, 0));
	
	GetCharacterMovement()->RotationRate = FRotator(0, 720, 0);
}

void ACEnemy::BeginPlay()
{
	Super::BeginPlay();

	Create_DynamicMaterial(this);//IICharacter�� �Լ� ���. ���� ����Ǵ� ��ü�� this(���⼭�� Enemy)�� ����.
	Change_Color(this, OriginColor);//���� �Ҵ�.

	State->OnStateTypeChanged.AddDynamic(this, &ACEnemy::OnStateTypeChanged);
}

void ACEnemy::OnStateTypeChanged(EStateType InPrevType, EStateType InNewType)
{
	Super::OnStateTypeChanged(InPrevType, InNewType);

	switch (InNewType)
	{
		case EStateType::Hitted: Hitted(); break;//����ó���� �Ʒ��� Hitted()���� �Ͼ��.
		case EStateType::Dead:   Dead(); break;//����ó���� �Ʒ��� Dead()���� �Ͼ��.
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