#pragma once
#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Kismet/KismetSystemLibrary.h"
#include "CFeetComponent.generated.h"

class ACharacter;

USTRUCT(BlueprintType)
struct FFeetData
{
	GENERATED_BODY()
		
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Feet")
	FVector LeftDistance; //X

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Feet")
	FVector RightDistance; //X

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Feet")
	FVector PelvisDistance; //Z

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Feet")
	FRotator LeftRotation;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Feet")
	FRotator RightRotation;
};

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class RPG_API UCFeetComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UCFeetComponent();
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	FORCEINLINE FFeetData GetData() { return Data; }

protected:
	virtual void BeginPlay() override;

private:
	void Trace(FName InName, float& OutDistance, FRotator& OutRotation);
	
	UPROPERTY(EditAnywhere, Category = "Trace")
	TEnumAsByte<EDrawDebugTrace::Type> DrawDebug;

	UPROPERTY(EditAnywhere, Category = "Trace")
	float InterpSpeed = 50;//���� ������ �� ������ �������� �ӵ�

	UPROPERTY(EditAnywhere, Category = "Trace")
	float TraceDistance = 50;//�������� Trace ����

	UPROPERTY(EditAnywhere, Category = "Trace")
	float OffsetDistance = 5;//������ ��¦ ���ִ� ��

	UPROPERTY(EditAnywhere, Category = "Socket")
	FName LeftSocket = "Foot_L";

	UPROPERTY(EditAnywhere, Category = "Socket")
	FName RightSocket = "Foot_R";
	
	ACharacter* OwnerCharacter;
	FFeetData Data;
};
