#include "Parkour/CParkourComponent.h"
#include "Global.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/ArrowComponent.h"

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

