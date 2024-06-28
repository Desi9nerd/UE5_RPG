#pragma once
#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Engine/DataTable.h"
#include "CParkourComponent.generated.h"

class UArrowComponent;

//������ ���� ������ ȭ��ǥ Ÿ��
UENUM(BlueprintType)
enum class EParkourArrowType : uint8
{
	Center = 0, Ceil, Floor, Left, Right, Land, Max,
};

//���� ���� Ÿ��
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
	void PlayMontage(class ACharacter* InCharacter);

	UPROPERTY(EditAnywhere)
	EParkourType Type;

	UPROPERTY(EditAnywhere)
	UAnimMontage* Montage;//���� Ÿ�Կ� ���� Play�� ��Ÿ��

	UPROPERTY(EditAnywhere)
	float PlayRatio = 1;//Play �ӵ�

	UPROPERTY(EditAnywhere)
	FName SectionName;//��Ÿ�ֿ� Section�� �ָ� �ش� Section���� ���. �̸� ���� ����

	UPROPERTY(EditAnywhere)
	float MinDistance;//������ ����� �ּҰŸ�

	UPROPERTY(EditAnywhere)
	float MaxDistance;//������ ����� �ִ�Ÿ�

	UPROPERTY(EditAnywhere)
	float Extent;//����, ��Ȳ�� ���� �ٸ��� ���

	UPROPERTY(EditAnywhere)
	bool bFixedCamera;//ī�޶� ���� ����
};

UCLASS()
class RPG_API UCParkourComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	FORCEINLINE bool IsExecuting() { return Type != EParkourType::Max; }
	
	UCParkourComponent();
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	//���� ���� �Լ�
	void DoParkour(bool bLanded = false);
	void End_DoParkour();

protected:
	virtual void BeginPlay() override;
	
private:
	bool Check_Obstacle();
	void LineTrace(EParkourArrowType InType);

	//********************************************************
	//** Arrow�� �� LineTrace�� üũ�ϴ� �Լ���
	void CheckTrace_Center();
	void CheckTrace_Ceil();
	void CheckTrace_Floor();
	void CheckTrace_LeftRight();
	void CheckTrace_Land();
	//********************************************************

	//********************************************************
	//** Climb, Fall, Slide
	bool Check_ClimbMode();
	void DoParkour_Climb();
	void End_DoParkour_Climb();

	bool Check_FallMode();
	void DoParkour_Fall();
	void End_DoParkour_Fall();

	bool Check_SlideMode();
	void DoParkour_Slide();
	void End_DoParkour_Slide();
	//********************************************************

	//********************************************************
	//** Short, Normal, Wall
	bool Check_ObstacleMode(EParkourType InType, FParkourData& OutData);
	void DoParkour_Obstacle(EParkourType InType, FParkourData& InData);
	void End_DoParkour_Obstacle();
	//********************************************************

	
	UPROPERTY(EditAnywhere, Category = "Data")
	UDataTable* DataTable;//���� DataTable

	UPROPERTY(EditAnywhere, Category = "Trace")
	float TraceDistance = 600.f;//���� Line Trace�� ����Ǵ� �Ÿ�

	UPROPERTY(EditAnywhere, Category = "Trace")
	TEnumAsByte<EDrawDebugTrace::Type> DebugType;

	UPROPERTY(EditAnywhere, Category = "Trace")
	float AvailableFrontAngle = 15.f;//���� ���� ���Ѱ���
	
	TMap<EParkourType, TArray<FParkourData>> DataMap;//TMap�� Key�� Key�� ������ �迭�� ���ϵȴ�.		
	
	ACharacter* OwnerCharacter;
	UArrowComponent* Arrows[(int32)EParkourArrowType::Max];
	FHitResult HitResults[(int32)EParkourArrowType::Max];//Arrows ���� �浹 ����� ������ �迭 ����
	
	AActor* HitObstacle;
	FVector HitObstacleExtent;
	float HitDistance;
	float ToFrontYaw;
	
	EParkourType Type = EParkourType::Max;//���� �������� ������ Ÿ��. �⺻���� Max�� �����Ͽ� �ƹ��͵� ���� �ʴ� Ÿ���� �⺻������ ������ش�.
	
	bool bFalling;
	
	AActor* BackupObstacle;//��ֹ��� ������ ��� ��� ����
};