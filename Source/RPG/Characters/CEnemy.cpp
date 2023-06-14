#include "CEnemy.h"
#include "Global.h"
#include "CAnimInstance.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/CMontagesComponent.h"
#include "Components/CMovementComponent.h"
#include "Components/CWeaponComponent.h"
#include "Components/CStatusComponent.h"
#include "Weapons/CWeaponStructures.h"

ACEnemy::ACEnemy()
{
	CHelpers::CreateActorComponent<UCWeaponComponent>(this, &Weapon, "Weapon");
	CHelpers::CreateActorComponent<UCMontagesComponent>(this, &Montages, "Montage");
	CHelpers::CreateActorComponent<UCMovementComponent>(this, &Movement, "Movement");
	CHelpers::CreateActorComponent<UCStateComponent>(this, &State, "State");
	CHelpers::CreateActorComponent<UCStatusComponent>(this, &Status, "Status");

	GetMesh()->SetRelativeLocation(FVector(0, 0, -90));
	GetMesh()->SetRelativeRotation(FRotator(0, -90, 0));

	USkeletalMesh* mesh;
	CHelpers::GetAsset<USkeletalMesh>(&mesh, "SkeletalMesh'/Game/Character/Mesh/SK_Mannequin.SK_Mannequin'");
	GetMesh()->SetSkeletalMesh(mesh);

	TSubclassOf<UCAnimInstance> animInstance;
	CHelpers::GetClass<UCAnimInstance>(&animInstance, "AnimBlueprint'/Game/ABP_Character.ABP_Character_C'");
	GetMesh()->SetAnimClass(animInstance);

	GetCharacterMovement()->RotationRate = FRotator(0, 720, 0);

}

void ACEnemy::BeginPlay()
{
	Super::BeginPlay();

	Movement->OnWalk();//�⺻�� ����.

	Create_DynamicMaterial(this);//IICharacter�� �Լ� ���. ���� ����Ǵ� ��ü�� this(���⼭�� Enemy)�� ����.
	Change_Color(this, OriginColor);//���� �Ҵ�.

	State->OnStateTypeChanged.AddDynamic(this, &ACEnemy::OnStateTypeChanged);
}

void ACEnemy::OnStateTypeChanged(EStateType InPrevType, EStateType InNewType)
{
  	switch (InNewType)
	{
		case EStateType::Hitted: Hitted(); break;//����ó���� �Ʒ��� Hitted()���� �Ͼ��.
		case EStateType::Dead:   Dead(); break;//����ó���� �Ʒ��� Dead()���� �Ͼ��.
	}
}
 
void ACEnemy::Hitted()
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
	if(!!Damage.Event && !!Damage.Event->HitData)
	{
		FHitData* data = Damage.Event->HitData;//FDamageData�� FActionDamageEvent* Event���� HitData

		data->PlayMontage(this);  //��Ÿ�� ���
		data->PlayHitStop(GetWorld());  //HitStop ���
		data->PlaySoundWave(this);//�Ҹ� ���
		data->PlayEffect(GetWorld(), GetActorLocation(), GetActorRotation());//Effect ���

		if (Status->IsDead() == false)
		{
			FVector start = GetActorLocation(); //start=���� ��ġ
			FVector target = Damage.Character->GetActorLocation();//target=�������� �����ϴ� ���
			FVector direction = target - start;//direction=target�� �ٶ󺸴� ����
			direction.Normalize();

			LaunchCharacter(-direction * data->Launch, false, false);
			SetActorRotation(UKismetMathLibrary::FindLookAtRotation(start, target));
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

void ACEnemy::End_Hitted()
{
	State->SetIdleMode();
}

void ACEnemy::RestoreColor()
{
	Change_Color(this, OriginColor);//���� ������ �����ش�.

	GetWorld()->GetTimerManager().ClearTimer(RestoreColor_TimerHandle);//RestoreColor_TimerHandle�� �ʱ�ȭ.
}

void ACEnemy::Dead()
{
	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);//Collision ����.

	Montages->PlayDeadMode();
}

void ACEnemy::End_Dead()
{
	Destroy(); //������ �Ҹ�����ش�.
}

float ACEnemy::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	float damage = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);

	Damage.Power = damage;
	Damage.Character = Cast<ACharacter>(EventInstigator->GetPawn());
	Damage.Causer = DamageCauser;
	Damage.Event = (FActionDamageEvent*)&DamageEvent;//���۷����� ĳ�����ϸ� ĳ������ �� �ǰų� ĳ���� �� ����� �� �Դ� ��찡 �߻��� �� �ִ�. //�����ϰ� C��Ÿ�� ĳ�����ϱ� ���� �����ͷ� ĳ�����Ѵ�. 

	State->SetHittedMode();//HittedMode�� ����.

	return damage;
}


