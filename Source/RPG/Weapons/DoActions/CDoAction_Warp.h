#pragma once

#include "CoreMinimal.h"
#include "Weapons/CDoAction.h"
#include "CDoAction_Warp.generated.h"

UCLASS(Blueprintable)
class RPG_API UCDoAction_Warp : public UCDoAction
{
	GENERATED_BODY()

public:
    UCDoAction_Warp();

    virtual void BeginPlay
    (
        class ACAttachment* InAttachment,
        class UCEquipment* InEquipment,
        class ACharacter* InOwner,
        const TArray<FDoActionData>& InDoActionData,
        const TArray<FHitData>& InHitData
    );
    void Tick(float InDeltaTime) override;

public:
    void DoAction() override;
    void Begin_DoAction() override;

private:
    bool GetCursorLocationAndRotation(FVector& OutLocation, FRotator& OutRotation);

private:
    class APlayerController* PlayerController;
    class UDecalComponent* Decal;

private:
    FVector MoveToLocation;
};
