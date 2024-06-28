#pragma once
#include "CoreMinimal.h"
#include "Kismet/GameplayStaticsTypes.h"
#include "Weapons/CAttachment.h"
#include "CAttachment_Bow.generated.h"

class ACPlayer;
class USplineComponent;
class USplineMeshComponent;
class UPoseableMeshComponent;

class UCStateComponent;

UCLASS()
class RPG_API ACAttachment_Bow : public ACAttachment
{
	GENERATED_BODY()

public:
    float* GetBend();
    
    ACAttachment_Bow();
    virtual void Tick(float DeltaTime) override;

    void OnBeginEquip_Implementation() override;
    void OnUnequip_Implementation() override;

protected:
    virtual void BeginPlay() override;

private:
    UPROPERTY(EditDefaultsOnly, Category = "View")
	FVector2D ViewPitchRange = FVector2D(-40, +30);//Pitch ���Ѱ� ����

    UPROPERTY(VisibleAnywhere)
	USkeletalMeshComponent* SkeletalMesh;

    UPROPERTY(VisibleAnywhere)
    UPoseableMeshComponent* PoseableMesh;

    UPROPERTY(VisibleAnywhere)
	USplineComponent* ArrowPathSpline;//ȭ�� ���� Spline

    UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent* ArcEndSphere;//ȭ�� ���� ����Ʈ

    UPROPERTY(EditAnywhere)
	UMaterial* ArrowPathSplineMaterial;

    UPROPERTY(EditAnywhere)
	UStaticMesh* ArrowPathSplineMesh;//ȭ�� ���� �Ž�

    FVector2D OriginViewPitchRange;


//---------------------------------------------------------
//-- ȭ�� ����
protected:
    UPROPERTY(EditAnywhere, Category = "ArrowSpeed")
	float ArrowSpeed = 5000.f;

    UPROPERTY()
	ACPlayer* PlayerCharacterCast;

    FVector CrosshairWorldLocation;
    FVector ImpactPoint;
    FVector TargetArrowSpawnLocation;
    FRotator TargetArrowSpawnRotation;

    FVector ArrowSpawnLocation;
    FRotator ArrowSpawnRotation;

    TArray<USplineMeshComponent*> SplineMeshes;

private:
    void GetStartAndEndforTrace();
    void GetArrowSpawnLocationAndRotation();
    void ClearArc();
    FPredictProjectilePathResult ProjectilePath();
    void UpdateArcSpline(FPredictProjectilePathResult InPredictResult);

    FHitResult TraceHitResult;//Arrow ��� LineTrace�� HitResults
    FVector FinalArcLocation;

    UCStateComponent* state;
//---------------------------------------------------------

};
