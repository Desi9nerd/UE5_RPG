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
    TWeakObjectPtr<ACBaseCharacter> Target;//타겟

private:
    UFUNCTION()
	void OnAttachmentBeginOverlap(ACharacter* InAttacker, AActor* InAttackCauser, ACharacter* InOther);//충돌 함수

    UPROPERTY(EditDefaultsOnly, Category = "Trace")
	float Distance = 1000.f;//이동거리

    UPROPERTY(EditDefaultsOnly, Category = "Trace")
	float Speed = 200.f;//속도

    UPROPERTY(EditDefaultsOnly, Category = "Trace")
	TEnumAsByte<EDrawDebugTrace::Type> DrawDebug;//디버그용
    
    UPROPERTY(EditDefaultsOnly, Category = "Action")
	FDoActionData ActionData;

    UPROPERTY(EditDefaultsOnly, Category = "Action")
	FHitData HitData;
    
    UPROPERTY(EditAnywhere, Category = "Add-On")
	TSubclassOf<class ACGhostTrail> GhostTrailClass;

    bool bMoving;

    FVector Start;
    FVector End;

    //복구를 위해 저장해두는 변수: Overlapped, Hitted
    TArray<ACharacter*> Overlapped;
    TArray<ACharacter*> Hitted;
    
    ACGhostTrail* GhostTrail;
};
