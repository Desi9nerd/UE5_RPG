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

	Movement->OnWalk();//기본값 설정.

	Create_DynamicMaterial(this);//IICharacter의 함수 사용. 색이 적용되는 객체를 this(여기서는 Enemy)로 설정.
	Change_Color(this, OriginColor);//색상 할당.

	State->OnStateTypeChanged.AddDynamic(this, &ACEnemy::OnStateTypeChanged);
}

void ACEnemy::OnStateTypeChanged(EStateType InPrevType, EStateType InNewType)
{
  	switch (InNewType)
	{
		case EStateType::Hitted: Hitted(); break;//실제처리는 아래의 Hitted()에서 일어난다.
		case EStateType::Dead:   Dead(); break;//실제처리는 아래의 Dead()에서 일어난다.
	}
}
 
void ACEnemy::Hitted()
{
	//Apply Damage 데미지 처리
	{   //다음 타격 때 꼬이지 않게 하기위해 초기화 해준다.
		Status->Damage(Damage.Power);
		Damage.Power = 0;
	}
	
	//Change Color
	{	
		Change_Color(this, FLinearColor::Red);

		FTimerDelegate timerDelegate;
		timerDelegate.BindUFunction(this, "RestoreColor");

		GetWorld()->GetTimerManager().SetTimer(RestoreColor_TimerHandle, timerDelegate, 0.2f, false);//0.2초 동안 반복없이 실행.
	}
	
	//Damage는 CEnemy.h의 FDamageData이다.
	if(!!Damage.Event && !!Damage.Event->HitData)
	{
		FHitData* data = Damage.Event->HitData;//FDamageData의 FActionDamageEvent* Event내의 HitData

		data->PlayMontage(this);  //몽타주 재생
		data->PlayHitStop(GetWorld());  //HitStop 재생
		data->PlaySoundWave(this);//소리 재생
		data->PlayEffect(GetWorld(), GetActorLocation(), GetActorRotation());//Effect 재생

		if (Status->IsDead() == false)
		{
			FVector start = GetActorLocation(); //start=현재 위치
			FVector target = Damage.Character->GetActorLocation();//target=데미지를 전달하는 대상
			FVector direction = target - start;//direction=target을 바라보는 방향
			direction.Normalize();

			LaunchCharacter(-direction * data->Launch, false, false);
			SetActorRotation(UKismetMathLibrary::FindLookAtRotation(start, target));
		}
	}

	//사망 처리
	if (Status->IsDead())
	{
		State->SetDeadMode();//State을 DeadMode로 만들어준다.

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
	Change_Color(this, OriginColor);//원래 색으로 돌려준다.

	GetWorld()->GetTimerManager().ClearTimer(RestoreColor_TimerHandle);//RestoreColor_TimerHandle를 초기화.
}

void ACEnemy::Dead()
{
	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);//Collision 꺼줌.

	Montages->PlayDeadMode();
}

void ACEnemy::End_Dead()
{
	Destroy(); //죽으면 소멸시켜준다.
}

float ACEnemy::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	float damage = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);

	Damage.Power = damage;
	Damage.Character = Cast<ACharacter>(EventInstigator->GetPawn());
	Damage.Causer = DamageCauser;
	Damage.Event = (FActionDamageEvent*)&DamageEvent;//레퍼런스로 캐스팅하면 캐스팅이 안 되거나 캐스팅 후 상속이 안 먹는 경우가 발생할 수 있다. //안전하게 C스타일 캐스팅하기 위해 포인터로 캐스팅한다. 

	State->SetHittedMode();//HittedMode로 변경.

	return damage;
}


