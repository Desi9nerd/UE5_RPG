#pragma once
#include "CoreMinimal.h"
#include "Characters/CBaseCharacter.h"
#include "Weapons/CSubAction.h"
#include "Weapons/CWeaponStructures.h"
#include "Kismet/KismetSystemLibrary.h"
#include "CSubAction_Sword.generated.h"

UCLASS(Blueprintable)
class RPG_API UCSubAction_Sword : public UCSubAction
{
	GENERATED_BODY()

public:
    virtual void BeginPlay(ACharacter* InOwner, ACAttachment* InAttachment, UCDoAction* InDoAction) override;

    void Pressed() override;
    void Begin_SubAction_Implementation() override;
    void End_SubAction_Implementation() override;
    void Tick_Implementation(float InDeltaTime) override;

     
    UCTargetComponent* Targeting; //Targeting  
    TWeakObjectPtr<ACBaseCharacter> Target;//Ÿ��

private:
    UFUNCTION()
	void OnAttachmentBeginOverlap(ACharacter* InAttacker, AActor* InAttackCauser, ACharacter* InOther);//�浹 �Լ�

    UPROPERTY(EditDefaultsOnly, Category = "Trace")
	float Distance = 1000.f;//�̵��Ÿ�

    UPROPERTY(EditDefaultsOnly, Category = "Trace")
	float Speed = 200.f;//�ӵ�

    UPROPERTY(EditDefaultsOnly, Category = "Trace")
	TEnumAsByte<EDrawDebugTrace::Type> DrawDebug;//����׿�
    
    UPROPERTY(EditDefaultsOnly, Category = "Action")
	FDoActionData ActionData;

    UPROPERTY(EditDefaultsOnly, Category = "Action")
	FHitData HitData;
    
    UPROPERTY(EditAnywhere, Category = "Add-On")
	TSubclassOf<class ACGhostTrail> GhostTrailClass;

    bool bMoving;

    FVector Start;
    FVector End;

    //������ ���� �����صδ� ����: Overlapped, Hitted
    TArray<ACharacter*> Overlapped;
    TArray<ACharacter*> Hitted;
    
    ACGhostTrail* GhostTrail;
};
