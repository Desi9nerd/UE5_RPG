#include "Weapons/AddOns/CThornObject.h"
#include "Global.h"
#include "NiagaraComponent.h"
#include "GameFramework/Character.h"

ACThornObject::ACThornObject()
{
	CHelpers::CreateComponent<USceneComponent>(this, &Root, TEXT("Root"));//Root ����
	CHelpers::CreateComponent<UNiagaraComponent>(this, &Niagara, TEXT("Niagara"), Root);//���̾ư��� ������Ʈ ����
}

void ACThornObject::BeginPlay()
{
	Super::BeginPlay();

	Niagara->SetNiagaraVariableObject("Collision", this);
	Niagara->OnSystemFinished.AddDynamic(this, &ACThornObject::OnSystemFinished);

	if (IsValid(NiagaraMesh))//NiagaraMesh�� �ִٸ�
	{
		FBox box = NiagaraMesh->GetBoundingBox();//NiagaraMesh �浹ü ������
		BoxExtent = (box.Min - box.Max).GetAbs() * 0.5f;//���Ƿ� ��ȯ�ؼ� ���.
	}
}

void ACThornObject::ReceiveParticleData_Implementation(const TArray<FBasicParticleData>& Data,
	UNiagaraSystem* NiagaraSystem, const FVector& SimulationPositionOffset)
{
	INiagaraParticleCallbackHandler::ReceiveParticleData_Implementation(Data, NiagaraSystem, SimulationPositionOffset);


	CheckFalse(Data.Num() > 0);

	//�ڽ� ���� //static�� ����ϴ�����?
	//�� ������ ���� Tick ��� �����Ͱ� �Ź� �Ҵ�ǰ� �������� stack���� ��� ���δ�. stack �����Ͱ� Ŀ���°� �����ϱ� ���� static���� ����Ѵ�. static�� �� �� �Ҵ��ϸ� ���Ű� �Ͼ�� �ʱ� ������ �����ϴ�. ��� �ݺ� �Ҵ�Ǿ�� �Ǵ°͵��� static���� ����ϸ� ����.
	static TArray<AActor*> ignores;
	ignores.AddUnique(GetOwner());

	static FHitResult hitResult;
	for (int32 i = Data.Num() - 1; i >= 0; i--)//�Ųٷ� ����� ����? �߰��� ���� �����Ǵ� ��� TArray�� �������� ������ �߻��� �� �����Ƿ� �Ųٷ� ó���Ѵ�.
	{
		FVector position = Data[i].Position + GetActorLocation();
		FVector scale = Data[i].Velocity * BoxExtent;

		UKismetSystemLibrary::BoxTraceSingleByProfile(GetWorld(), position, position, scale, NiagaraMeshRotation, "Pawn", false, ignores, EDrawDebugTrace::ForOneFrame, hitResult, true);//Pawn ����, �����浹 false,

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
	Destroy();//���̾ư��� �����Ѵ�.
}

