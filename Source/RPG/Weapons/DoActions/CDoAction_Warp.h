#pragma once
#include "CoreMinimal.h"
#include "Weapons/CDoAction.h"
#include "CDoAction_Warp.generated.h"

class UCAIBehaviorComponent;

UCLASS(Blueprintable)
class RPG_API UCDoAction_Warp : public UCDoAction
{
	GENERATED_BODY()

public:
    virtual void BeginPlay
    (
        class ACAttachment* InAttachment,
        class UCEquipment* InEquipment,
        class ACharacter* InOwner,
        const TArray<FDoActionData>& InDoActionData,
        const TArray<FHitData>& InHitData
    );
    void Tick(float InDeltaTime) override;
    
    void DoAction() override;
    void Begin_DoAction() override;

private:
    bool GetCursorLocationAndRotation(FVector& OutLocation, FRotator& OutRotation);
    
    APlayerController* PlayerController;
    UDecalComponent* Decal;
    UCAIBehaviorComponent* Behavior;//UCAIBehaviorComponent(=Behavior 변수) 유무에 따라 Enemy인지 Player인지 구분할 수 있다.
    
    FVector MoveToLocation;
};
