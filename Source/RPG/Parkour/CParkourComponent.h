#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Kismet/KismetSystemLibrary.h"//DrawDebug�� �����ϴ� ���
#include "Engine/DataTable.h"//DataTable�� �ٷ�� ���
#include "CParkourComponent.generated.h"

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

public:
	void PlayMontage(class ACharacter* InCharacter);
};

UCLASS()
class RPG_API UCParkourComponent : public UActorComponent
{
	GENERATED_BODY()

private:
	UPROPERTY(EditAnywhere, Category = "Data")
		UDataTable* DataTable;//���� DataTable

private:
	UPROPERTY(EditAnywhere, Category = "Trace")
		float TraceDistance = 600;//���� Line Trace�� ����Ǵ� �Ÿ�

	UPROPERTY(EditAnywhere, Category = "Trace")
		TEnumAsByte<EDrawDebugTrace::Type> DebugType;

	UPROPERTY(EditAnywhere, Category = "Trace")
		float AvailableFrontAngle = 15;//���� ���� ���Ѱ���

public:
	UCParkourComponent();

protected:
	virtual void BeginPlay() override;

public:
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

private:
	void LineTrace(EParkourArrowType InType);

private:
	//Arrow�� �� LineTrace�� üũ�ϴ� �Լ���
	void CheckTrace_Center();
	void CheckTrace_Ceil();
	void CheckTrace_Floor();
	void CheckTrace_LeftRight();

private:
	bool Check_Obstacle();

public:
	//���� ���� �Լ�
	void DoParkour(bool bLanded = false);
	void End_DoParkour();

private:
	bool Check_ClimbMode();
	void DoParkour_Climb();
	void End_DoParkour_Climb();

private:
	TMap<EParkourType, TArray<FParkourData>> DataMap;//TMap�� Key�� Key�� ������ �迭�� ���ϵȴ�.		

private:
	class ACharacter* OwnerCharacter;
	class UArrowComponent* Arrows[(int32)EParkourArrowType::Max];

	FHitResult HitResults[(int32)EParkourArrowType::Max];//Arrows ���� �浹 ����� ������ �迭 ����

private:
	AActor* HitObstacle;
	FVector HitObstacleExtent;
	float HitDistance;
	float ToFrontYaw;

private:
	EParkourType Type = EParkourType::Max;//���� �������� ������ Ÿ��. �⺻���� Max�� �����Ͽ� �ƹ��͵� ���� �ʴ� Ÿ���� �⺻������ ������ش�. 
};
