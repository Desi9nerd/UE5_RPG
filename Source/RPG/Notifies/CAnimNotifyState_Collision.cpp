#include "CAnimNotifyState_Collision.h"
#include "EngineUtils.h"
#include "Global.h"
#include "Characters/CBaseCharacter.h"
#include "Components/CWeaponComponent.h"
#include "Weapons/CAttachment.h"

FString UCAnimNotifyState_Collision::GetNotifyName_Implementation() const
{
	return "Collision";
}

void UCAnimNotifyState_Collision::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyBegin(MeshComp, Animation, TotalDuration, EventReference);
	CheckNull(MeshComp);
	CheckNull(MeshComp->GetOwner());

	UCWeaponComponent* weapon = CHelpers::GetComponent<UCWeaponComponent>(MeshComp->GetOwner());
	CheckNull(weapon);
	CheckNull(weapon->GetAttachment());

	weapon->GetAttachment()->OnCollisions();
}

void UCAnimNotifyState_Collision::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyEnd(MeshComp, Animation, EventReference);
	CheckNull(MeshComp);
	CheckNull(MeshComp->GetOwner());

	UCWeaponComponent* weapon = CHelpers::GetComponent<UCWeaponComponent>(MeshComp->GetOwner());
	CheckNull(weapon);
	CheckNull(weapon->GetAttachment());

	weapon->GetAttachment()->OffCollisions();
}

void UCAnimNotifyState_Collision::NotifyTick(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
	float FrameDeltaTime)
{
	Super::NotifyTick(MeshComp, Animation, FrameDeltaTime);

	FVector Start = MeshComp->GetSocketLocation(StartSocketName);
	FVector End = MeshComp->GetSocketLocation(EndSocketName);

	FHitResult hitResult;
	TArray<TEnumAsByte<EObjectTypeQuery>> objects;
	objects.Add(EObjectTypeQuery::ObjectTypeQuery3);//ObjectTypeQuery3�� Pawn. Pawn�� �����ϱ� ���� objects�� �߰��Ͽ���
	TArray<AActor*> ignores;//���ܽ�ų �͵��� ���� �迭 ����

	ACBaseCharacter* MyCharacter = Cast<ACBaseCharacter>(MeshComp->GetOwner());
	if (MyCharacter == nullptr)
		return; 

	int32 MyTeamID = MyCharacter->GetGenericTeamId();// CBaseCharacter�� �Լ��� �����ϴ� ����� TeamID�� ��´�. 

	for (TActorIterator<APawn> It(MyCharacter->GetWorld()); It; ++It) 
	{
		ACBaseCharacter* OtherChar = Cast<ACBaseCharacter>(*It);

		if (OtherChar && OtherChar->GetGenericTeamId() == MyTeamID)// TeamID�� ���ٸ�  
		{
			ignores.Add(OtherChar);// ignores�� ���� �� ĳ���͵��� ��´�
			continue;
		}
	}

	bool condition = UKismetSystemLibrary::SphereTraceSingleForObjects(
		MeshComp,
		Start,
		End,
		140.0f,
		objects,
		false,
		ignores,
		EDrawDebugTrace::None,
		hitResult,
		true
	);

	if(condition)
	{		
		UGameplayStatics::SpawnEmitterAtLocation( // Blood �� Ƣ���
			MeshComp,
			HitEffectBlood,
			hitResult.ImpactPoint,
			UKismetMathLibrary::MakeRotFromXY(hitResult.Normal, hitResult.Normal),
			FVector(0.4f, 0.4f, 0.4f),
			true
		);		
		UGameplayStatics::SpawnEmitterAtLocation( // ���� Particle 
			MeshComp,
			HitEffectParticleSystem,
			hitResult.ImpactPoint,
			UKismetMathLibrary::MakeRotFromX(hitResult.ImpactNormal),
			ParticleScale,
			true
		);		
		UNiagaraFunctionLibrary::SpawnSystemAtLocation( // ���� Particle - Niagara
			MeshComp,
			HitEffectNiagaraSystem,
			hitResult.ImpactPoint,
			UKismetMathLibrary::MakeRotFromX(hitResult.ImpactNormal),
			NiagaraScale,
			true
		);
	}

} 