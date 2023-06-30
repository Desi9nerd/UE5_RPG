#pragma once

#include "CoreMinimal.h"
#include "Weapons/CSubAction.h"
#include "CSubAction_Warp.generated.h"

UCLASS(Blueprintable)
class RPG_API UCSubAction_Warp : public UCSubAction
{
	GENERATED_BODY()

private:
    UPROPERTY(EditDefaultsOnly, Category = "Camera")
        TSubclassOf<class ACameraActor> CameraActorClass;

    UPROPERTY(EditDefaultsOnly, Category = "Camera")
        FVector CameraRelativeLocation = FVector(0, 0, 1000);//플레이어와 카메라 사이의 상대간격

    UPROPERTY(EditDefaultsOnly, Category = "Camera")
        TEnumAsByte<ECameraProjectionMode::Type> ProjectionMode;//Projection모드: perspective, orthographic

    UPROPERTY(EditDefaultsOnly, Category = "Camera")
        float OrthoWidth = 2000;

    UPROPERTY(EditDefaultsOnly, Category = "Camera")
        float FieldOfView = 90;

    UPROPERTY(EditDefaultsOnly, Category = "Camera")
        float BlendIn = 0;//Player카메라에서 Warp카메라로 전환되는 시간

    UPROPERTY(EditDefaultsOnly, Category = "Camera")
        float BlendOut = 0;//Warp카메라에서 Player카메라로 전환되는 시간

public:
    UCSubAction_Warp();

public:
    virtual void Pressed() override;
    virtual void Released() override;

public:
    void BeginPlay(class ACharacter* InOwner, class ACAttachment* InAttachment, class UCDoAction* InDoAction) override;

public:
    void Tick_Implementation(float InDeltaTime) override;

private:
    class APlayerController* PlayerController;
    class ACameraActor* CameraActor;
};

