#include "Weapons/AddOns/CThornObject.h"
#include "Global.h"
#include "NiagaraComponent.h"
#include "GameFramework/Character.h"

ACThornObject::ACThornObject()
{
	CHelpers::CreateComponent<USceneComponent>(this, &Root, TEXT("Root"));//Root 생성
	CHelpers::CreateComponent<UNiagaraComponent>(this, &Niagara, TEXT("Niagara"), Root);//나이아가라 컴포넌트 생성
}

void ACThornObject::BeginPlay()
{
	Super::BeginPlay();

	Niagara->SetNiagaraVariableObject("Collision", this);
	Niagara->OnSystemFinished.AddDynamic(this, &ACThornObject::OnSystemFinished);

	if (IsValid(NiagaraMesh))//NiagaraMesh가 있다면
	{
		FBox box = NiagaraMesh->GetBoundingBox();//NiagaraMesh 충돌체 사이즈
		BoxExtent = (box.Min - box.Max).GetAbs() * 0.5f;//부피로 전환해서 사용.
	}
}

void ACThornObject::ReceiveParticleData_Implementation(const TArray<FBasicParticleData>& Data,
	UNiagaraSystem* NiagaraSystem, const FVector& SimulationPositionOffset)
{
	INiagaraParticleCallbackHandler::ReceiveParticleData_Implementation(Data, NiagaraSystem, SimulationPositionOffset);


	CheckFalse(Data.Num() > 0);

	//박스 추적 //static을 사용하는이유?
	//매 프레임 마다 Tick 결과 데이터가 매번 할당되고 지워져서 stack으로 계속 쌓인다. stack 데이터가 커지는걸 방지하기 위해 static으로 사용한다. static은 한 번 할당하면 제거가 일어나지 않기 때문에 유리하다. 계속 반복 할당되어야 되는것들은 static으로 사용하면 좋다.
	static TArray<AActor*> ignores;
	ignores.AddUnique(GetOwner());

	static FHitResult hitResult;
	for (int32 i = Data.Num() - 1; i >= 0; i--)//거꾸로 사용한 이유? 중간에 먼저 삭제되는 경우 TArray가 땡겨져서 문제가 발생할 수 있으므로 거꾸로 처리한다.
	{
		FVector position = Data[i].Position + GetActorLocation();
		FVector scale = Data[i].Velocity * BoxExtent;

		UKismetSystemLibrary::BoxTraceSingleByProfile(GetWorld(), position, position, scale, NiagaraMeshRotation, "Pawn", false, ignores, EDrawDebugTrace::ForOneFrame, hitResult, true);//Pawn 추적, 복합충돌 false,

		if (hitResult.bBlockingHit)
		{
			if (IsValid(CollisionEffect))
			{
				FTransform transform = CollisionEffectTransform;
				transform.AddToTranslation(hitResult.Location);

				CHelpers::PlayEffect(GetWorld(), CollisionEffect, transform);
			}

			ACharacter* character = Cast<ACharacter>(hitResult.GetActor());
			if (IsValid(character))
			{
				HitData.SendDamage(Cast<ACharacter>(GetOwner()), this, character);
			}
		}
	}
}

void ACThornObject::OnSystemFinished(UNiagaraComponent* PSystem)
{
	Destroy();//나이아가라를 제거한다.
}

