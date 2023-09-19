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
    UCSubAction_Sword();

    virtual void BeginPlay(ACharacter* InOwner, ACAttachment* InAttachment, UCDoAction* InDoAction) override;
    
private:
    UPROPERTY(EditDefaultsOnly, Category = "Trace")
        float Distance = 1000.0;//이동거리

    UPROPERTY(EditDefaultsOnly, Category = "Trace")
        float Speed = 200;//속도

    UPROPERTY(EditDefaultsOnly, Category = "Trace")
        TEnumAsByte<EDrawDebugTrace::Type> DrawDebug;//디버그용
    
    UPROPERTY(EditDefaultsOnly, Category = "Action")
        FDoActionData ActionData;

    UPROPERTY(EditDefaultsOnly, Category = "Action")
        FHitData HitData;

private:
    UPROPERTY(EditAnywhere, Category = "Add-On")
        TSubclassOf<class ACGhostTrail> GhostTrailClass;

public:
    void Pressed() override;
    void Begin_SubAction_Implementation() override;
    void End_SubAction_Implementation() override;
    void Tick_Implementation(float InDeltaTime) override;

private:
    UFUNCTION()
        void OnAttachmentBeginOverlap(class ACharacter* InAttacker, AActor* InAttackCauser, class ACharacter* InOther);//충돌 함수

private:
    bool bMoving;

    FVector Start;
    FVector End;

    //복구를 위해 저장해두는 변수: Overlapped, Hitted
    TArray<class ACharacter*> Overlapped;
    TArray<class ACharacter*> Hitted;

private:
    class ACGhostTrail* GhostTrail;

//Targeting    
public:
    class UCTargetComponent* Targeting;

    TWeakObjectPtr<ACBaseCharacter> Target;//타겟

};
