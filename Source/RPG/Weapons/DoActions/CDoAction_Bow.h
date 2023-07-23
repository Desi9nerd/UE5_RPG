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
    void End_BowString();//AnimNotify_BowString�� Notify���� ���Ͽ� ����Ѵ�.

/** ȭ�� */
private:
    void CreateArrow();//ȭ�� ����
    class ACArrow* GetAttachedArrow();//Ȱ�� ���� ȭ���� ���ϴ� �Լ�.

private:
    UFUNCTION()
        void OnArrowHit(class AActor* InCauser, class ACharacter* InOtherCharacter);

    UFUNCTION()
        void OnArrowEndPlay(class ACArrow* InDestroyer);
    /** ȭ�� */

private:
    class USkeletalMeshComponent* SkeletalMesh;
    class UPoseableMeshComponent* PoseableMesh;

private:
    float* Bending;

private:
    FVector OriginLocation;//ó�� ��ġ
    bool bAttachedString = true;//true�� Ȱ ������ �տ� �ٴ´�.

private:
     const bool* bEquipped;
     
private:
    TArray<class ACArrow*> Arrows;//ȭ���(=ȭ�� ���)


/** ȭ�� ����
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
    FHitResult HitResult;//Arrow ��� LineTrace�� HitResults
};
