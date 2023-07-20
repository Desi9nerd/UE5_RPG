#include "Parkour/CParkourComponent.h"
#include "Global.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/ArrowComponent.h"
#include "Components/CMovementComponent.h"

//#define LOG_UCParkourComponent

void FParkourData::PlayMontage(class ACharacter* InCharacter)
{
	//파쿠르 동작 중에는 FixedCamera를 켜서 카메라가 움직이지 않게 해준다.
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


	CheckTrace_Land();//아래로 쏘는 Land Arrow의 LineTrace는 매 Tick 검사해야 한다.
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

void UCParkourComponent::CheckTrace_Land()
{
	CheckFalse(OwnerCharacter->GetCharacterMovement()->IsFalling());

	//최초 추락
	CheckTrue(bFalling);
	bFalling = true;

	UArrowComponent* arrow = Arrows[(int32)EParkourArrowType::Land];//Land Arrow
	FLinearColor color = FLinearColor(arrow->ArrowColor);

	FTransform transform = arrow->GetComponentToWorld();
	FVector start = transform.GetLocation();//Land Arrow 위치

	const TArray<FParkourData>* datas = DataMap.Find(EParkourType::Fall);
	FVector end = start + transform.GetRotation().GetForwardVector() * (*datas)[0].Extent;//Land Arrow 위치에서 Extent만큼 늘어난 거리를 end 지점으로 설정.


	TArray<AActor*> ignores;//LineTrace를 무시할 Actor들을 위한 변수
	ignores.Add(OwnerCharacter);//OwnerCharacter인 Player를 무시하도록 설정.

	UKismetSystemLibrary::LineTraceSingle(GetWorld(), start, end, ETraceTypeQuery::TraceTypeQuery1, false, ignores, DebugType, HitResults[(int32)EParkourArrowType::Land], true, color, FLinearColor::White);
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

	if (bLanded && Check_FallMode())
	{
		DoParkour_Fall();

		return;
	}

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


	CheckFalse(Check_Obstacle());//장애물이 있는지 체크

	if (Check_ClimbMode())//올라가기 파쿠르를 수행할 조건이 된다면
	{
		DoParkour_Climb();//올라가기 파쿠르 수행

		return;
	}

	if (Check_SlideMode())//슬라이드 파쿠르를 수행할 조건이 된다면
	{
		DoParkour_Slide();//슬라이드 파쿠르 수행

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
	switch (Type)//현재 수행중인 EParkourType
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

	Type = EParkourType::Max;//EParkourType을 원래대로 돌려준다.

	//파쿠르가 끝난 후에 FixedCamera를 꺼준다.
	UCMovementComponent* movement = CHelpers::GetComponent<UCMovementComponent>(OwnerCharacter);
	if (!!movement)
		movement->DisableFixedCamera();
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

bool UCParkourComponent::Check_FallMode()
{
	CheckFalseResult(bFalling, false);//bFalling=false면 바로 return false;
	bFalling = false;

	float distance = HitResults[(int32)EParkourArrowType::Land].Distance;

	const TArray<FParkourData>* datas = DataMap.Find(EParkourType::Fall);
	//거리 판단
	CheckFalseResult((*datas)[0].MinDistance < distance, false);//파쿠르가 수행될 최소거리<distance면 바로 return false;
	CheckFalseResult((*datas)[0].MaxDistance > distance, false);//파쿠르가 수행될 최대거리>distance면 바로 return false;

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
	//충돌한게 슬라이드 거리 안에 들어와있는지 판단
	CheckTrueResult(HitResults[(int32)EParkourArrowType::Floor].bBlockingHit, false);

	const TArray<FParkourData>* datas = DataMap.Find(EParkourType::Slide);
	CheckFalseResult((*datas)[0].MinDistance < HitDistance, false);
	CheckFalseResult((*datas)[0].MaxDistance > HitDistance, false);

	UArrowComponent* arrow = Arrows[(int32)EParkourArrowType::Floor];
	FLinearColor color = FLinearColor(arrow->ArrowColor);

	FTransform transform = arrow->GetComponentToWorld();

	FVector arrowLocation = transform.GetLocation();//Floor Arrow 위치
	FVector ownerLocation = OwnerCharacter->GetActorLocation();//캐릭터의 위치


	float extent = (*datas)[0].Extent;//DataTable의 Extent 정보로 넣어준다.
	float z = arrowLocation.Z + extent;//z = Floor Arrow의 z값 + extent 값

	FVector forward = OwnerCharacter->GetActorForwardVector();//캐릭터의 전방벡터
	forward = FVector(forward.X, forward.Y, 0);//캐릭터의 전방벡터에서 z값만 0으로 만든다.

	FVector start = FVector(arrowLocation.X, arrowLocation.Y, z);//start위치 = (Floor Arrow의 X, Y 위치값)과 (Floor Arrow의 z값 + extent 값)
	FVector end = start + forward * TraceDistance;//end위치 = start위치 + forward * TraceDistance


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
	BackupObstacle->SetActorEnableCollision(false);//장애물의 Collision을 꺼준다.
}

void UCParkourComponent::End_DoParkour_Slide()
{
	BackupObstacle->SetActorEnableCollision(true);//장애물의 Collision을 다시 켜준다.
	BackupObstacle = NULL;
}

bool UCParkourComponent::Check_ObstacleMode(EParkourType InType, FParkourData& OutData)
{
	CheckTrueResult(HitResults[(int32)EParkourArrowType::Ceil].bBlockingHit, false);//Ceil Arrow의 LineTrace에 충돌된게 없다면 return false로 끝낸다.

	const TArray<FParkourData>* datas = DataMap.Find(InType);//InType의 데이터를 넣어준다. InType은 Short, Normal, Wall 중 하나일 것이다.

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