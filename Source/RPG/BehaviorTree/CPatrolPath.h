#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CPatrolPath.generated.h"

UCLASS()
class RPG_API ACPatrolPath : public AActor
{
	GENERATED_BODY()

private:
	UPROPERTY(EditAnywhere, Category = "Loop")
		bool bLoop;

	UPROPERTY(EditAnywhere, Category = "Path")
		int32 Index;

	UPROPERTY(EditAnywhere, Category = "Path")
		bool bReverse;

	UPROPERTY(VisibleDefaultsOnly)
		class USceneComponent* Root;

	UPROPERTY(VisibleDefaultsOnly)
		class USplineComponent* Spline;

	UPROPERTY(VisibleDefaultsOnly)
		class UTextRenderComponent* Text;

public:
	FORCEINLINE class USplineComponent* GetSpline() { return Spline; }
	
	ACPatrolPath();
	void OnConstruction(const FTransform& Transform) override;

	FVector GetMoveTo();
	void UpdateIndex();

protected:
	virtual void BeginPlay() override;
};
