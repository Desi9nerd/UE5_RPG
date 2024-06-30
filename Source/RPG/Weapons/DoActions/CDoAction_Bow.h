#pragma once
#include "CoreMinimal.h"
#include "Weapons/CDoAction.h"
#include "CDoAction_Bow.generated.h"

class ACArrow;
class UPoseableMeshComponent;

UCLASS(Blueprintable)
class RPG_API UCDoAction_Bow : public UCDoAction
{
	GENERATED_BODY()

public:
    void BeginPlay
    (
        class ACAttachment* InAttachment,
        class UCEquipment* InEquipment,
        class ACharacter* InOwner,
        const TArray<FDoActionData>& InDoActionData,
        const TArray<FHitData>& InHitData
    ) override;

    void Tick(float InDeltaTime) override;

    void DoAction() override;
    void Begin_DoAction() override;
    void End_DoAction() override;

    void OnBeginEquip() override;
    void OnUnequip() override;
    
    void End_BowString();//AnimNotify_BowString의 Notify에서 콜하여 사용한다.

private:
    //*****************************************************
    //** 화살
    void CreateArrow();//화살 생성
    ACArrow* GetAttachedArrow();//활에 붙은 화살을 콜하는 함수.
    
    UFUNCTION()
	void OnArrowHit(AActor* InCauser, ACharacter* InOtherCharacter);

    UFUNCTION()
	void OnArrowEndPlay(ACArrow* InDestroyer);
    //*****************************************************


    UPROPERTY(EditDefaultsOnly, Category = "Arrow")
	TSubclassOf<ACArrow> ArrowClass;
    
    USkeletalMeshComponent* SkeletalMesh;
    UPoseableMeshComponent* PoseableMesh;
    
    float* Bending;
    
    FVector OriginLocation;//처음 위치
    bool bAttachedString = true;//true면 활 시위가 손에 붙는다.

	const bool* bEquipped;
     
    TArray<class ACArrow*> Arrows;//화살들(=화살 목록)
};
