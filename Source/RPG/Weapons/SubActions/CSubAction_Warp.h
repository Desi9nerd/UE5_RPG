#pragma once
#include "CoreMinimal.h"
#include "Weapons/CSubAction.h"
#include "CSubAction_Warp.generated.h"

class APlayerController;
class ACameraActor;

UCLASS(Blueprintable)
class RPG_API UCSubAction_Warp : public UCSubAction
{
	GENERATED_BODY()

public:
    UCSubAction_Warp();
    void BeginPlay(class ACharacter* InOwner, class ACAttachment* InAttachment, class UCDoAction* InDoAction) override;
    void Tick_Implementation(float InDeltaTime) override;
    
    virtual void Pressed() override;
    virtual void Released() override;

private:
    UPROPERTY(EditDefaultsOnly, Category = "Camera")
	TSubclassOf<ACameraActor> CameraActorClass;

    UPROPERTY(EditDefaultsOnly, Category = "Camera")
	FVector CameraRelativeLocation = FVector(0.f, 0.f, 1000.f);//플레이어와 카메라 사이의 상대간격

    UPROPERTY(EditDefaultsOnly, Category = "Camera")
	TEnumAsByte<ECameraProjectionMode::Type> ProjectionMode;//Projection모드: perspective, orthographic

    UPROPERTY(EditDefaultsOnly, Category = "Camera")
	float OrthoWidth = 2000.f;

    UPROPERTY(EditDefaultsOnly, Category = "Camera")
	float FieldOfView = 90.f;

    UPROPERTY(EditDefaultsOnly, Category = "Camera")
    float BlendIn = 0.f;//Player카메라에서 Warp카메라로 전환되는 시간

    UPROPERTY(EditDefaultsOnly, Category = "Camera")
	float BlendOut = 0.f;//Warp카메라에서 Player카메라로 전환되는 시간

    APlayerController* PlayerController;
    ACameraActor* CameraActor;
};

