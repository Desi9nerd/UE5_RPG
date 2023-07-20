#include "Parkour/CParkourComponent.h"
#include "Global.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/ArrowComponent.h"
#include "Components/CMovementComponent.h"

//#define LOG_UCParkourComponent

void FParkourData::PlayMontage(class ACharacter* InCharacter)
{
	//���� ���� �߿��� FixedCamera�� �Ѽ� ī�޶� �������� �ʰ� ���ش�.
	if (bFixedCamera)
	{
		UCMovementComponent* movement = CHelpers::GetComponent<UCMovementComponent>(InCharacter);

		if (!!movement)
			movement->EnableFixedCamera();
	}

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


	CheckTrace_Land();//�Ʒ��� ��� Land Arrow�� LineTrace�� �� Tick �˻��ؾ� �Ѵ�.
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

void UCParkourComponent::CheckTrace_Land()
{
	CheckFalse(OwnerCharacter->GetCharacterMovement()->IsFalling());

	//���� �߶�
	CheckTrue(bFalling);
	bFalling = true;

	UArrowComponent* arrow = Arrows[(int32)EParkourArrowType::Land];//Land Arrow
	FLinearColor color = FLinearColor(arrow->ArrowColor);

	FTransform transform = arrow->GetComponentToWorld();
	FVector start = transform.GetLocation();//Land Arrow ��ġ

	const TArray<FParkourData>* datas = DataMap.Find(EParkourType::Fall);
	FVector end = start + transform.GetRotation().GetForwardVector() * (*datas)[0].Extent;//Land Arrow ��ġ���� Extent��ŭ �þ �Ÿ��� end �������� ����.


	TArray<AActor*> ignores;//LineTrace�� ������ Actor���� ���� ����
	ignores.Add(OwnerCharacter);//OwnerCharacter�� Player�� �����ϵ��� ����.

	UKismetSystemLibrary::LineTraceSingle(GetWorld(), start, end, ETraceTypeQuery::TraceTypeQuery1, false, ignores, DebugType, HitResults[(int32)EParkourArrowType::Land], true, color, FLinearColor::White);
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

	if (bLanded && Check_FallMode())
	{
		DoParkour_Fall();

		return;
	}

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


	CheckFalse(Check_Obstacle());//��ֹ��� �ִ��� üũ

	if (Check_ClimbMode())//�ö󰡱� ������ ������ ������ �ȴٸ�
	{
		DoParkour_Climb();//�ö󰡱� ���� ����

		return;
	}

	if (Check_SlideMode())//�����̵� ������ ������ ������ �ȴٸ�
	{
		DoParkour_Slide();//�����̵� ���� ����

		return;
	}


	FParkourData data;
	if (Check_ObstacleMode(EParkourType::Normal, data))//Normal
	{
		DoParkour_Obstacle(EParkourType::Normal, data);

		return;
	}

	if (Check_ObstacleMode(EParkourType::Short, data))//Short
	{
		DoParkour_Obstacle(EParkourType::Short, data);

		return;
	}

	if (Check_ObstacleMode(EParkourType::Wall, data))//Wall
	{
		DoParkour_Obstacle(EParkourType::Wall, data);

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

	case EParkourType::Fall:
		End_DoParkour_Fall();
		break;

	case EParkourType::Slide:
		End_DoParkour_Slide();
		break;

	case EParkourType::Short:
	case EParkourType::Normal:
	case EParkourType::Wall:
		End_DoParkour_Obstacle();
		break;
	}

	Type = EParkourType::Max;//EParkourType�� ������� �����ش�.

	//������ ���� �Ŀ� FixedCamera�� ���ش�.
	UCMovementComponent* movement = CHelpers::GetComponent<UCMovementComponent>(OwnerCharacter);
	if (!!movement)
		movement->DisableFixedCamera();
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

bool UCParkourComponent::Check_FallMode()
{
	CheckFalseResult(bFalling, false);//bFalling=false�� �ٷ� return false;
	bFalling = false;

	float distance = HitResults[(int32)EParkourArrowType::Land].Distance;

	const TArray<FParkourData>* datas = DataMap.Find(EParkourType::Fall);
	//�Ÿ� �Ǵ�
	CheckFalseResult((*datas)[0].MinDistance < distance, false);//������ ����� �ּҰŸ�<distance�� �ٷ� return false;
	CheckFalseResult((*datas)[0].MaxDistance > distance, false);//������ ����� �ִ�Ÿ�>distance�� �ٷ� return false;

	return true;
}

void UCParkourComponent::DoParkour_Fall()
{
	Type = EParkourType::Fall;

	(*DataMap.Find(EParkourType::Fall))[0].PlayMontage(OwnerCharacter);
}

void UCParkourComponent::End_DoParkour_Fall()
{
}

bool UCParkourComponent::Check_SlideMode()
{
	//�浹�Ѱ� �����̵� �Ÿ� �ȿ� �����ִ��� �Ǵ�
	CheckTrueResult(HitResults[(int32)EParkourArrowType::Floor].bBlockingHit, false);

	const TArray<FParkourData>* datas = DataMap.Find(EParkourType::Slide);
	CheckFalseResult((*datas)[0].MinDistance < HitDistance, false);
	CheckFalseResult((*datas)[0].MaxDistance > HitDistance, false);

	UArrowComponent* arrow = Arrows[(int32)EParkourArrowType::Floor];
	FLinearColor color = FLinearColor(arrow->ArrowColor);

	FTransform transform = arrow->GetComponentToWorld();

	FVector arrowLocation = transform.GetLocation();//Floor Arrow ��ġ
	FVector ownerLocation = OwnerCharacter->GetActorLocation();//ĳ������ ��ġ


	float extent = (*datas)[0].Extent;//DataTable�� Extent ������ �־��ش�.
	float z = arrowLocation.Z + extent;//z = Floor Arrow�� z�� + extent ��

	FVector forward = OwnerCharacter->GetActorForwardVector();//ĳ������ ���溤��
	forward = FVector(forward.X, forward.Y, 0);//ĳ������ ���溤�Ϳ��� z���� 0���� �����.

	FVector start = FVector(arrowLocation.X, arrowLocation.Y, z);//start��ġ = (Floor Arrow�� X, Y ��ġ��)�� (Floor Arrow�� z�� + extent ��)
	FVector end = start + forward * TraceDistance;//end��ġ = start��ġ + forward * TraceDistance


	TArray<AActor*> ignores;
	FHitResult hitResult;

	UKismetSystemLibrary::BoxTraceSingle(GetWorld(), start, end, FVector(0, extent, extent), OwnerCharacter->GetActorRotation(), ETraceTypeQuery::TraceTypeQuery1, false, ignores, DebugType, hitResult, true);
	CheckTrueResult(hitResult.bBlockingHit, false);

	return true;
}

void UCParkourComponent::DoParkour_Slide()
{
	Type = EParkourType::Slide;

	OwnerCharacter->SetActorRotation(FRotator(0, ToFrontYaw, 0));
	(*DataMap.Find(EParkourType::Slide))[0].PlayMontage(OwnerCharacter);

	BackupObstacle = HitObstacle;
	BackupObstacle->SetActorEnableCollision(false);//��ֹ��� Collision�� ���ش�.
}

void UCParkourComponent::End_DoParkour_Slide()
{
	BackupObstacle->SetActorEnableCollision(true);//��ֹ��� Collision�� �ٽ� ���ش�.
	BackupObstacle = NULL;
}

bool UCParkourComponent::Check_ObstacleMode(EParkourType InType, FParkourData& OutData)
{
	CheckTrueResult(HitResults[(int32)EParkourArrowType::Ceil].bBlockingHit, false);//Ceil Arrow�� LineTrace�� �浹�Ȱ� ���ٸ� return false�� ������.

	const TArray<FParkourData>* datas = DataMap.Find(InType);//InType�� �����͸� �־��ش�. InType�� Short, Normal, Wall �� �ϳ��� ���̴�.

	for (int32 i = 0; i < (*datas).Num(); i++)
	{
		bool b = true;
		b &= (*datas)[i].MinDistance < HitDistance;
		b &= (*datas)[i].MaxDistance > HitDistance;
		b &= FMath::IsNearlyEqual((*datas)[i].Extent, HitObstacleExtent.Y, 10);

		OutData = (*datas)[i];
		CheckTrueResult(b, true);
	}

	return false;
}

void UCParkourComponent::DoParkour_Obstacle(EParkourType InType, FParkourData& InData)
{
	Type = InType;//Short, Normal, Wall

	OwnerCharacter->SetActorRotation(FRotator(0, ToFrontYaw, 0));
	InData.PlayMontage(OwnerCharacter);

	BackupObstacle = HitObstacle;
	BackupObstacle->SetActorEnableCollision(false);
}

void UCParkourComponent::End_DoParkour_Obstacle()
{
	BackupObstacle->SetActorEnableCollision(true);
	BackupObstacle = NULL;
}