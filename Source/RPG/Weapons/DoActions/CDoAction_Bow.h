#pragma once

#include "CoreMinimal.h"
#include "Weapons/CDoAction.h"
#include "CDoAction_Bow.generated.h"

class ACPlayer;
class USplineMeshComponent;

UCLASS(Blueprintable)
class RPG_API UCDoAction_Bow : public UCDoAction
{
	GENERATED_BODY()

private:
    UPROPERTY(EditDefaultsOnly, Category = "Arrow")
        TSubclassOf<class ACArrow> ArrowClass;

public:
    UCDoAction_Bow();

    void BeginPlay
    (
        class ACAttachment* InAttachment,
        class UCEquipment* InEquipment,
        class ACharacter* InOwner,
        const TArray<FDoActionData>& InDoActionData,
        const TArray<FHitData>& InHitData
    ) override;

    void DoAction() override;
    void Begin_DoAction() override;
    void End_DoAction() override;

    void OnBeginEquip() override;
    void OnUnequip() override;

    void Tick(float InDeltaTime) override;

public:
    void End_BowString();//AnimNotify_BowString의 Notify에서 콜하여 사용한다.

/** 화살 */
private:
    void CreateArrow();//화살 생성
    class ACArrow* GetAttachedArrow();//활에 붙은 화살을 콜하는 함수.

private:
    UFUNCTION()
        void OnArrowHit(class AActor* InCauser, class ACharacter* InOtherCharacter);

    UFUNCTION()
        void OnArrowEndPlay(class ACArrow* InDestroyer);
    /** 화살 */

private:
    class USkeletalMeshComponent* SkeletalMesh;
    class UPoseableMeshComponent* PoseableMesh;

private:
    float* Bending;

private:
    FVector OriginLocation;//처음 위치
    bool bAttachedString = true;//true면 활 시위가 손에 붙는다.

private:
     const bool* bEquipped;
     
private:
    TArray<class ACArrow*> Arrows;//화살들(=화살 목록)


/** 화살 궤적
 * 
 */
private:
    void GetStartAndEndforTrace();
    void GetArrowSpawnLocationAndRotation();
    void ClearArc();
    void ProjectilePath();
    void UpdateArcSpline();

protected:
    FVector CrosshairWorldLocation;
    FVector ImpactPoint;
    FVector TargetArrowSpawnLocation;
    FRotator TargetArrowSpawnRotation;

    FVector ArrowSpawnLocation;
    FRotator ArrowSpawnRotation;
    
    TArray<USplineMeshComponent*> SplineMeshes;

    UPROPERTY(EditAnywhere, Category = "ArrowSpeed")
        float ArrowSpeed;

private:
    ACPlayer* PlayerCharacter;
    FHitResult HitResult;//Arrow 경로 LineTrace의 HitResults
};
