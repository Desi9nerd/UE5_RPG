#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Kismet/KismetSystemLibrary.h"//DrawDebug를 포함하는 헤더
#include "Engine/DataTable.h"//DataTable를 다루는 헤더
#include "CParkourComponent.generated.h"

//파쿠르를 위해 추적할 화살표 타입
UENUM(BlueprintType)
enum class EParkourArrowType : uint8
{
	Center = 0, Ceil, Floor, Left, Right, Land, Max,
};

//파쿠르 동작 타입
UENUM(BlueprintType)
enum class EParkourType : uint8
{
	Climb = 0, Fall, Slide, Short, Normal, Wall, Max,
};

USTRUCT(BlueprintType)
struct FParkourData : public FTableRowBase
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere)
		EParkourType Type;

	UPROPERTY(EditAnywhere)
		UAnimMontage* Montage;//파쿠르 타입에 따라 Play할 몽타주

	UPROPERTY(EditAnywhere)
		float PlayRatio = 1;//Play 속도

	UPROPERTY(EditAnywhere)
		FName SectionName;//몽타주에 Section을 주면 해당 Section부터 재생. 이를 위한 변수

	UPROPERTY(EditAnywhere)
		float MinDistance;//파쿠르가 수행될 최소거리

	UPROPERTY(EditAnywhere)
		float MaxDistance;//파쿠르가 수행될 최대거리

	UPROPERTY(EditAnywhere)
		float Extent;//부피, 상황에 따라 다르게 사용

	UPROPERTY(EditAnywhere)
		bool bFixedCamera;//카메라 고정 여부

public:
	void PlayMontage(class ACharacter* InCharacter);
};

UCLASS()
class RPG_API UCParkourComponent : public UActorComponent
{
	GENERATED_BODY()

private:
	UPROPERTY(EditAnywhere, Category = "Data")
		UDataTable* DataTable;//파쿠르 DataTable

private:
	UPROPERTY(EditAnywhere, Category = "Trace")
		float TraceDistance = 600;//파쿠르 Line Trace가 적용되는 거리

	UPROPERTY(EditAnywhere, Category = "Trace")
		TEnumAsByte<EDrawDebugTrace::Type> DebugType;

	UPROPERTY(EditAnywhere, Category = "Trace")
		float AvailableFrontAngle = 15;//파쿠르 수행 제한각도

public:
	UCParkourComponent();

protected:
	virtual void BeginPlay() override;

public:
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

private:
	void LineTrace(EParkourArrowType InType);

private:
	//Arrow로 쏜 LineTrace를 체크하는 함수들
	void CheckTrace_Center();
	void CheckTrace_Ceil();
	void CheckTrace_Floor();
	void CheckTrace_LeftRight();

private:
	bool Check_Obstacle();

public:
	//파쿠르 수행 함수
	void DoParkour(bool bLanded = false);
	void End_DoParkour();

private:
	bool Check_ClimbMode();
	void DoParkour_Climb();
	void End_DoParkour_Climb();

private:
	TMap<EParkourType, TArray<FParkourData>> DataMap;//TMap에 Key와 Key를 넣으면 배열이 리턴된다.		

private:
	class ACharacter* OwnerCharacter;
	class UArrowComponent* Arrows[(int32)EParkourArrowType::Max];

	FHitResult HitResults[(int32)EParkourArrowType::Max];//Arrows 마다 충돌 결과를 저장할 배열 변수

private:
	AActor* HitObstacle;
	FVector HitObstacleExtent;
	float HitDistance;
	float ToFrontYaw;

private:
	EParkourType Type = EParkourType::Max;//현재 수행중인 파쿠프 타입. 기본값을 Max로 설정하여 아무것도 하지 않는 타입을 기본값으로 만들어준다. 
};
