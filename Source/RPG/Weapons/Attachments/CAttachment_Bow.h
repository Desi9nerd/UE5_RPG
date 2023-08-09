#pragma once

#include "CoreMinimal.h"
#include "Kismet/GameplayStaticsTypes.h"
#include "Weapons/CAttachment.h"
#include "CAttachment_Bow.generated.h"

class ACPlayer;
class USplineMeshComponent;

UCLASS()
class RPG_API ACAttachment_Bow : public ACAttachment
{
	GENERATED_BODY()

private:
    UPROPERTY(EditDefaultsOnly, Category = "View")
        FVector2D ViewPitchRange = FVector2D(-40, +30);//Pitch ���Ѱ� ����

private:
    UPROPERTY(VisibleAnywhere)
        class USkeletalMeshComponent* SkeletalMesh;

    UPROPERTY(VisibleAnywhere)
        class UPoseableMeshComponent* PoseableMesh;

    UPROPERTY(VisibleAnywhere)
        class USplineComponent* ArrowPathSpline;//ȭ�� ���� Spline\

    UPROPERTY(VisibleAnywhere)
        class UStaticMeshComponent* ArcEndSphere;//ȭ�� ���� ����Ʈ

    UPROPERTY(VisibleAnywhere)
        class USplineMeshComponent* ArrowPathSplineMesh;//ȭ�� ���� �Ž�

public:
    float* GetBend();

public:
    ACAttachment_Bow();

protected:
    virtual void BeginPlay() override;

public:
    virtual void Tick(float DeltaTime) override;

public:
    void OnBeginEquip_Implementation() override;
    void OnUnequip_Implementation() override;

private:
    FVector2D OriginViewPitchRange;


/** ȭ�� ����
 *
 */
private:
    void GetStartAndEndforTrace();
    void GetArrowSpawnLocationAndRotation();
    void ClearArc();
    FPredictProjectilePathResult ProjectilePath();
    void UpdateArcSpline(FPredictProjectilePathResult InPredictResult);

protected:
    FVector CrosshairWorldLocation;
    FVector ImpactPoint;
    FVector TargetArrowSpawnLocation;
    FRotator TargetArrowSpawnRotation;

    FVector ArrowSpawnLocation;
    FRotator ArrowSpawnRotation;

    TArray<USplineMeshComponent*> SplineMeshes;

    UPROPERTY(EditAnywhere, Category = "ArrowSpeed")
        float ArrowSpeed = 2500.0f;

private:
    ACPlayer* PlayerCharacterCast;
    FHitResult TraceHitResult;//Arrow ��� LineTrace�� HitResults
    //FPredictProjectilePathResult PredictResult;
    FVector FinalArcLocation;
    
};
