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
    
    void End_BowString();//AnimNotify_BowString�� Notify���� ���Ͽ� ����Ѵ�.

private:
    //*****************************************************
    //** ȭ��
    void CreateArrow();//ȭ�� ����
    ACArrow* GetAttachedArrow();//Ȱ�� ���� ȭ���� ���ϴ� �Լ�.
    
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
    
    FVector OriginLocation;//ó�� ��ġ
    bool bAttachedString = true;//true�� Ȱ ������ �տ� �ٴ´�.

	const bool* bEquipped;
     
    TArray<class ACArrow*> Arrows;//ȭ���(=ȭ�� ���)
};
