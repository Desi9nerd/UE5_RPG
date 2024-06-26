#pragma once
#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Characters/ECharacterTypes.h"
#include "Engine/DataTable.h"
#include "CMontagesComponent.generated.h"

USTRUCT()
struct FMontageData : public FTableRowBase
{
	GENERATED_BODY()
		
	UPROPERTY(EditAnywhere)
	EStateType Type; //상태 Type

	UPROPERTY(EditAnywhere)
	class UAnimMontage* Montage;

	UPROPERTY(EditAnywhere)
	float PlayRate = 1; //Player 속도
};

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class RPG_API UCMontagesComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	void PlayDodgeMode();
	void PlayDeadMode();

protected:
	virtual void BeginPlay() override;

private:
	void PlayAnimMontage(EStateType InType);
	
	UPROPERTY(EditAnywhere, Category = "DataTable")
	UDataTable* DataTable;

	ACharacter* OwnerCharacter;
	FMontageData* Datas[(int32)EStateType::Max];
};
