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

	if (!!HitObstacle)//HitObstacle이 있다면(=CheckTrace_Center의 hitResult가 있다면)
	{
		//나머지 Arrow들로도 LineTrace 검사한다.
		CheckTrace_Ceil();
		CheckTrace_Floor();
		CheckTrace_LeftRight();
	}
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
	CheckNullResult(HitObstacle, false);//HitObstacle이 null이 아닌지 체크

	//Arrow Center, Left, Right 모두 Hit되는지 체크.
	bool b = true;
	b &= HitResults[(int32)EParkourArrowType::Center].bBlockingHit;
	b &= HitResults[(int32)EParkourArrowType::Left].bBlockingHit;
	b &= HitResults[(int32)EParkourArrowType::Right].bBlockingHit;
	CheckFalseResult(b, false);

	//Arrow Center, Left, Right의 Normal이 모두 같은 방향인지 체크. 모서리인지 아닌지 판단
	FVector center = HitResults[(int32)EParkourArrowType::Center].Normal;
	FVector left = HitResults[(int32)EParkourArrowType::Left].Normal;
	FVector right = HitResults[(int32)EParkourArrowType::Right].Normal;

	CheckFalseResult(center.Equals(left), false);//Arrow center와 left값이 같은지 체크
	CheckFalseResult(center.Equals(right), false);//Arrow center와 right값이 같은지 체크

	//impactNormal과 player가 바라보는 사이의 각도를 구하여 AvailableFrontAngle 각도보다 작을 때 수행하게 만든다.
	FVector start = HitResults[(int32)EParkourArrowType::Center].ImpactPoint;//Hit된 지점
	FVector end = OwnerCharacter->GetActorLocation();//Player의 위치
	float lookAt = UKismetMathLibrary::FindLookAtRotation(start, end).Yaw;//Player가 Hit된 위치를 바라보는 방향의 Yaw값을 구한다.

	FVector impactNormal = HitResults[(int32)EParkourArrowType::Center].ImpactNormal;//Hit된 지점의 Normal
	float impactAt = UKismetMathLibrary::MakeRotFromX(impactNormal).Yaw;//Normal 방향벡터의 Yaw값

	float yaw = abs(abs(lookAt) - abs(impactAt));//두 개의 Yaw값의 차이(=impactNormal과 player가 바라보는 사이의 각도)

	CheckFalseResult(yaw <= AvailableFrontAngle, false);//AvailableFrontAngle로 설정한 각 15도 이하라면

	return true;//수행한다.
}

void UCParkourComponent::DoParkour(bool bLanded)
{
	CheckFalse(Type == EParkourType::Max);//Max는 파쿠르가 수행중이 아닌 상황

	CheckFalse(Check_Obstacle());//장애물이 있는지 체크

	if (Check_ClimbMode())//올라가기 파쿠르를 수행할 조건이 된다면
	{
		DoParkour_Climb();//올라가기 파쿠르 수행

		return;
	}
}

void UCParkourComponent::End_DoParkour()
{
	switch (Type)//현재 수행중인 EParkourType
	{
	case EParkourType::Climb:
		End_DoParkour_Climb();
		break;
	}

	Type = EParkourType::Max;//EParkourType을 원래대로 돌려준다.
}

bool UCParkourComponent::Check_ClimbMode()
{
	CheckFalseResult(HitResults[(int32)EParkourArrowType::Ceil].bBlockingHit, false);

	const TArray<FParkourData>* datas = DataMap.Find(EParkourType::Climb);//Find는 값이 아닌 포인터를 리턴한다. const를 사용해서 고칠 수 없도록 만든다.
	CheckFalseResult((*datas)[0].MinDistance < HitDistance, false);//datas접근해서 0번의 MinDistance가 HitDistance보다 작은지 체크.
	CheckFalseResult((*datas)[0].MaxDistance > HitDistance, false);//datas접근해서 0번의 MaxDistance가 HitDistance보다 큰지 체크.
	CheckFalseResult(FMath::IsNearlyEqual((*datas)[0].Extent, HitObstacleExtent.Z, 10), false);//datas접근해서 0번의 Extent가 HitObstacleExtent.Z값과 같은지 체크.

	return true;
}

void UCParkourComponent::DoParkour_Climb()
{
	Type = EParkourType::Climb;//EParkourType을 Climb으로 설정.

	OwnerCharacter->SetActorLocation(HitResults[(int32)EParkourArrowType::Center].ImpactPoint);//Player를 Arrow Center가 쏜 LineTrace에 Hit된 위치로 이동시킨다.
	OwnerCharacter->SetActorRotation(FRotator(0, ToFrontYaw, 0));//Player가 Hit된 방향을 바라보게 한다.
	(*DataMap.Find(EParkourType::Climb))[0].PlayMontage(OwnerCharacter);//Climb 몽타주를 재생

	OwnerCharacter->GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Flying);//기어올라가야 하므로 중력을 꺼줘야한다. 그래서 MovementMode를 MOVE_Flying로 변경한다.
}

void UCParkourComponent::End_DoParkour_Climb()
{
	OwnerCharacter->GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Walking);
}