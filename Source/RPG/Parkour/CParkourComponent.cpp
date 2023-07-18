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

	CHelpers::GetAsset<UDataTable>(&DataTable, "DataTable'/Game/Parkour/DT_Parkour.DT_Parkour'");//DataTable 생성.
}

void UCParkourComponent::BeginPlay()
{
	Super::BeginPlay();

	TArray<FParkourData*> datas;//FParkourData 사용하여 파쿠르 관련 변수들을 담는 구조체 사용.
	DataTable->GetAllRows<FParkourData>("", datas);//DataTable에서 데이터를 가져온다.

	for (int32 i = 0; i < (int32)EParkourType::Max; i++)
	{
		TArray<FParkourData> temp;//FParkourData 배열 temp 생성.
		for (FParkourData* data : datas)//datas를 순회하여 검색
		{
			if (data->Type == (EParkourType)i)//데이터의 타입 == UENUM인 EParkourType 이라면
				temp.Add(*data);//해당 파쿠르 타입을 temp에 담는다. 

			DataMap.Add((EParkourType)i, temp);//DataMap에 Key에 파쿠르 타입을 숫자로, Value에 FParkourData 배열 temp를 담는다.
		}
	}


	OwnerCharacter = Cast<ACharacter>(GetOwner());//해당 클래스를 가지고 있는 GetOwner인 캐릭터(여기서는 Player) OwnerCharacter로 설정.

	USceneComponent* arrow = CHelpers::GetComponent<USceneComponent>(OwnerCharacter, "ArrowGroup");//Arrows를 가져온다(=OwnerCharacter의 ArrowGroup을 가져온다).

	TArray<USceneComponent*> components;//components 변수에 SceneComponent들(=Arrows)을 가져와 담는다.
	arrow->GetChildrenComponents(false, components);

	for (int32 i = 0; i < (int32)EParkourArrowType::Max; i++)
		Arrows[i] = Cast<UArrowComponent>(components[i]);//Arrows[] 배열 변수에 components인 Arrows를 담는다.
}

void UCParkourComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	//값 초기화
	HitObstacle = NULL;
	HitObstacleExtent = FVector::ZeroVector;
	HitDistance = 0;
	ToFrontYaw = 0;

	CheckTrace_Center();
}

void UCParkourComponent::LineTrace(EParkourArrowType InType)
{
	UArrowComponent* arrow = Arrows[(int32)InType];//Trace할 화살을 가져온다.
	FLinearColor color = FLinearColor(arrow->ArrowColor);

	FTransform transform = arrow->GetComponentToWorld();//arrow의 위치를 가져온다.

	//위치를 활용하여 길이를 만든다.
	FVector start = transform.GetLocation();
	FVector end = start + OwnerCharacter->GetActorForwardVector() * TraceDistance;


	TArray<AActor*> ignores;//Line Trace 시 무시할 것들을 담는 변수
	ignores.Add(OwnerCharacter);//OwnerCharacter는 Line Trace에서 무시되게 한다.

	UKismetSystemLibrary::LineTraceSingle(GetWorld(), start, end, ETraceTypeQuery::TraceTypeQuery3, false, ignores, DebugType, HitResults[(int32)InType], true, color, FLinearColor::White);//TraceTypeQuery3는 새로 만든 TraceChannel인 Parkour다.
}

void UCParkourComponent::CheckTrace_Center()
{
	EParkourArrowType type = EParkourArrowType::Center;
	LineTrace(type);

	const FHitResult& hitResult = HitResults[(int32)type];
	CheckFalse(hitResult.bBlockingHit);

	//StaticMesh가 충돌체가 되므로 StaticMesh를 변수로 두고 StaticMesh와의 충돌 여부를 체크한다.
	UStaticMeshComponent* mesh = CHelpers::GetComponent<UStaticMeshComponent>(hitResult.GetActor());
	CheckNull(mesh);


	HitObstacle = hitResult.GetActor();

	FVector minBound, maxBound;
	mesh->GetLocalBounds(minBound, maxBound);//부피

	float x = FMath::Abs(minBound.X - maxBound.X);
	float y = FMath::Abs(minBound.Y - maxBound.Y);
	float z = FMath::Abs(minBound.Z - maxBound.Z);
	HitObstacleExtent = FVector(x, y, z);

	HitDistance = hitResult.Distance;//충돌체까지의 거리

	//충돌체에 대한 Normal을 뒤집는다. Player는 밖에서 안을 바라보기 때문에 -로 값을 뒤집는다.
	ToFrontYaw = UKismetMathLibrary::MakeRotFromX(-hitResult.ImpactNormal).Yaw;

#ifdef LOG_UCParkourComponent
	CLog::Print(HitObstacle, 10);
	CLog::Print(HitObstacleExtent, 11);
	CLog::Print(HitDistance, 12);
	CLog::Print(ToFrontYaw, 13);
#endif //LOG_UCParkourComponent
}

