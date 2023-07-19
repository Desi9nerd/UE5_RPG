#include "Parkour/CParkourComponent.h"
#include "Global.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/ArrowComponent.h"

#define LOG_UCParkourComponent

void FParkourData::PlayMontage(class ACharacter* InCharacter)
{
	InCharacter->PlayAnimMontage(Montage, PlayRatio, SectionName);
}

///////////////////////////////////////////////////////////////////////////////

UCParkourComponent::UCParkourComponent()
{
	PrimaryComponentTick.bCanEverTick = true;

	CHelpers::GetAsset<UDataTable>(&DataTable, "DataTable'/Game/Parkour/DT_Parkour.DT_Parkour'");//DataTable ����.
}

void UCParkourComponent::BeginPlay()
{
	Super::BeginPlay();

	TArray<FParkourData*> datas;//FParkourData ����Ͽ� ���� ���� �������� ��� ����ü ���.
	DataTable->GetAllRows<FParkourData>("", datas);//DataTable���� �����͸� �����´�.

	for (int32 i = 0; i < (int32)EParkourType::Max; i++)
	{
		TArray<FParkourData> temp;//FParkourData �迭 temp ����.
		for (FParkourData* data : datas)//datas�� ��ȸ�Ͽ� �˻�
		{
			if (data->Type == (EParkourType)i)//�������� Ÿ�� == UENUM�� EParkourType �̶��
				temp.Add(*data);//�ش� ���� Ÿ���� temp�� ��´�. 

			DataMap.Add((EParkourType)i, temp);//DataMap�� Key�� ���� Ÿ���� ���ڷ�, Value�� FParkourData �迭 temp�� ��´�.
		}
	}


	OwnerCharacter = Cast<ACharacter>(GetOwner());//�ش� Ŭ������ ������ �ִ� GetOwner�� ĳ����(���⼭�� Player) OwnerCharacter�� ����.

	USceneComponent* arrow = CHelpers::GetComponent<USceneComponent>(OwnerCharacter, "ArrowGroup");//Arrows�� �����´�(=OwnerCharacter�� ArrowGroup�� �����´�).

	TArray<USceneComponent*> components;//components ������ SceneComponent��(=Arrows)�� ������ ��´�.
	arrow->GetChildrenComponents(false, components);

	for (int32 i = 0; i < (int32)EParkourArrowType::Max; i++)
		Arrows[i] = Cast<UArrowComponent>(components[i]);//Arrows[] �迭 ������ components�� Arrows�� ��´�.
}

void UCParkourComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	//�� �ʱ�ȭ
	HitObstacle = NULL;
	HitObstacleExtent = FVector::ZeroVector;
	HitDistance = 0;
	ToFrontYaw = 0;

	CheckTrace_Center();

	if (!!HitObstacle)//HitObstacle�� �ִٸ�(=CheckTrace_Center�� hitResult�� �ִٸ�)
	{
		//������ Arrow��ε� LineTrace �˻��Ѵ�.
		CheckTrace_Ceil();
		CheckTrace_Floor();
		CheckTrace_LeftRight();
	}
}

void UCParkourComponent::LineTrace(EParkourArrowType InType)
{
	UArrowComponent* arrow = Arrows[(int32)InType];//Trace�� ȭ���� �����´�.
	FLinearColor color = FLinearColor(arrow->ArrowColor);

	FTransform transform = arrow->GetComponentToWorld();//arrow�� ��ġ�� �����´�.

	//��ġ�� Ȱ���Ͽ� ���̸� �����.
	FVector start = transform.GetLocation();
	FVector end = start + OwnerCharacter->GetActorForwardVector() * TraceDistance;


	TArray<AActor*> ignores;//Line Trace �� ������ �͵��� ��� ����
	ignores.Add(OwnerCharacter);//OwnerCharacter�� Line Trace���� ���õǰ� �Ѵ�.

	UKismetSystemLibrary::LineTraceSingle(GetWorld(), start, end, ETraceTypeQuery::TraceTypeQuery3, false, ignores, DebugType, HitResults[(int32)InType], true, color, FLinearColor::White);//TraceTypeQuery3�� ���� ���� TraceChannel�� Parkour��.
}

void UCParkourComponent::CheckTrace_Center()
{
	EParkourArrowType type = EParkourArrowType::Center;
	LineTrace(type);

	const FHitResult& hitResult = HitResults[(int32)type];
	CheckFalse(hitResult.bBlockingHit);

	//StaticMesh�� �浹ü�� �ǹǷ� StaticMesh�� ������ �ΰ� StaticMesh���� �浹 ���θ� üũ�Ѵ�.
	UStaticMeshComponent* mesh = CHelpers::GetComponent<UStaticMeshComponent>(hitResult.GetActor());
	CheckNull(mesh);


	HitObstacle = hitResult.GetActor();

	FVector minBound, maxBound;
	mesh->GetLocalBounds(minBound, maxBound);//����

	float x = FMath::Abs(minBound.X - maxBound.X);
	float y = FMath::Abs(minBound.Y - maxBound.Y);
	float z = FMath::Abs(minBound.Z - maxBound.Z);
	HitObstacleExtent = FVector(x, y, z);

	HitDistance = hitResult.Distance;//�浹ü������ �Ÿ�

	//�浹ü�� ���� Normal�� �����´�. Player�� �ۿ��� ���� �ٶ󺸱� ������ -�� ���� �����´�.
	ToFrontYaw = UKismetMathLibrary::MakeRotFromX(-hitResult.ImpactNormal).Yaw;

#ifdef LOG_UCParkourComponent
	CLog::Print(HitObstacle, 10);
	CLog::Print(HitObstacleExtent, 11);
	CLog::Print(HitDistance, 12);
	CLog::Print(ToFrontYaw, 13);
#endif //LOG_UCParkourComponent
}

void UCParkourComponent::CheckTrace_Ceil()
{
	LineTrace(EParkourArrowType::Ceil);
}

void UCParkourComponent::CheckTrace_Floor()
{
	LineTrace(EParkourArrowType::Floor);
}

void UCParkourComponent::CheckTrace_LeftRight()
{
	LineTrace(EParkourArrowType::Left);
	LineTrace(EParkourArrowType::Right);
}

bool UCParkourComponent::Check_Obstacle()
{
	CheckNullResult(HitObstacle, false);//HitObstacle�� null�� �ƴ��� üũ

	//Arrow Center, Left, Right ��� Hit�Ǵ��� üũ.
	bool b = true;
	b &= HitResults[(int32)EParkourArrowType::Center].bBlockingHit;
	b &= HitResults[(int32)EParkourArrowType::Left].bBlockingHit;
	b &= HitResults[(int32)EParkourArrowType::Right].bBlockingHit;
	CheckFalseResult(b, false);

	//Arrow Center, Left, Right�� Normal�� ��� ���� �������� üũ. �𼭸����� �ƴ��� �Ǵ�
	FVector center = HitResults[(int32)EParkourArrowType::Center].Normal;
	FVector left = HitResults[(int32)EParkourArrowType::Left].Normal;
	FVector right = HitResults[(int32)EParkourArrowType::Right].Normal;

	CheckFalseResult(center.Equals(left), false);//Arrow center�� left���� ������ üũ
	CheckFalseResult(center.Equals(right), false);//Arrow center�� right���� ������ üũ

	//impactNormal�� player�� �ٶ󺸴� ������ ������ ���Ͽ� AvailableFrontAngle �������� ���� �� �����ϰ� �����.
	FVector start = HitResults[(int32)EParkourArrowType::Center].ImpactPoint;//Hit�� ����
	FVector end = OwnerCharacter->GetActorLocation();//Player�� ��ġ
	float lookAt = UKismetMathLibrary::FindLookAtRotation(start, end).Yaw;//Player�� Hit�� ��ġ�� �ٶ󺸴� ������ Yaw���� ���Ѵ�.

	FVector impactNormal = HitResults[(int32)EParkourArrowType::Center].ImpactNormal;//Hit�� ������ Normal
	float impactAt = UKismetMathLibrary::MakeRotFromX(impactNormal).Yaw;//Normal ���⺤���� Yaw��

	float yaw = abs(abs(lookAt) - abs(impactAt));//�� ���� Yaw���� ����(=impactNormal�� player�� �ٶ󺸴� ������ ����)

	CheckFalseResult(yaw <= AvailableFrontAngle, false);//AvailableFrontAngle�� ������ �� 15�� ���϶��

	return true;//�����Ѵ�.
}

void UCParkourComponent::DoParkour(bool bLanded)
{
	CheckFalse(Type == EParkourType::Max);//Max�� ������ �������� �ƴ� ��Ȳ

	CheckFalse(Check_Obstacle());//��ֹ��� �ִ��� üũ

	if (Check_ClimbMode())//�ö󰡱� ������ ������ ������ �ȴٸ�
	{
		DoParkour_Climb();//�ö󰡱� ���� ����

		return;
	}
}

void UCParkourComponent::End_DoParkour()
{
	switch (Type)//���� �������� EParkourType
	{
	case EParkourType::Climb:
		End_DoParkour_Climb();
		break;
	}

	Type = EParkourType::Max;//EParkourType�� ������� �����ش�.
}

bool UCParkourComponent::Check_ClimbMode()
{
	CheckFalseResult(HitResults[(int32)EParkourArrowType::Ceil].bBlockingHit, false);

	const TArray<FParkourData>* datas = DataMap.Find(EParkourType::Climb);//Find�� ���� �ƴ� �����͸� �����Ѵ�. const�� ����ؼ� ��ĥ �� ������ �����.
	CheckFalseResult((*datas)[0].MinDistance < HitDistance, false);//datas�����ؼ� 0���� MinDistance�� HitDistance���� ������ üũ.
	CheckFalseResult((*datas)[0].MaxDistance > HitDistance, false);//datas�����ؼ� 0���� MaxDistance�� HitDistance���� ū�� üũ.
	CheckFalseResult(FMath::IsNearlyEqual((*datas)[0].Extent, HitObstacleExtent.Z, 10), false);//datas�����ؼ� 0���� Extent�� HitObstacleExtent.Z���� ������ üũ.

	return true;
}

void UCParkourComponent::DoParkour_Climb()
{
	Type = EParkourType::Climb;//EParkourType�� Climb���� ����.

	OwnerCharacter->SetActorLocation(HitResults[(int32)EParkourArrowType::Center].ImpactPoint);//Player�� Arrow Center�� �� LineTrace�� Hit�� ��ġ�� �̵���Ų��.
	OwnerCharacter->SetActorRotation(FRotator(0, ToFrontYaw, 0));//Player�� Hit�� ������ �ٶ󺸰� �Ѵ�.
	(*DataMap.Find(EParkourType::Climb))[0].PlayMontage(OwnerCharacter);//Climb ��Ÿ�ָ� ���

	OwnerCharacter->GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Flying);//���ö󰡾� �ϹǷ� �߷��� ������Ѵ�. �׷��� MovementMode�� MOVE_Flying�� �����Ѵ�.
}

void UCParkourComponent::End_DoParkour_Climb()
{
	OwnerCharacter->GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Walking);
}