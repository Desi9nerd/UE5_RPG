#pragma once
#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CPatrolPath.generated.h"

class USceneComponent;
class USplineComponent;
class UTextRenderComponent;

UCLASS()
class RPG_API ACPatrolPath : public AActor
{
	GENERATED_BODY()

public:
	FORCEINLINE class USplineComponent* GetSpline() { return Spline; }
	
	ACPatrolPath();
	void OnConstruction(const FTransform& Transform) override;

	FVector GetMoveTo();
	void UpdateIndex();

protected:
	virtual void BeginPlay() override;

private:
	UPROPERTY(EditAnywhere, Category = "Loop")
	bool bLoop;

	UPROPERTY(EditAnywhere, Category = "Path")
	int32 Index;

	UPROPERTY(EditAnywhere, Category = "Path")
	bool bReverse;

	UPROPERTY(VisibleDefaultsOnly)
	USceneComponent* Root;

	UPROPERTY(VisibleDefaultsOnly)
	USplineComponent* Spline;

	UPROPERTY(VisibleDefaultsOnly)
	UTextRenderComponent* Text;
};
