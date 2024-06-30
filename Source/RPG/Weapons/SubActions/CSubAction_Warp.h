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
	FVector CameraRelativeLocation = FVector(0.f, 0.f, 1000.f);//�÷��̾�� ī�޶� ������ ��밣��

    UPROPERTY(EditDefaultsOnly, Category = "Camera")
	TEnumAsByte<ECameraProjectionMode::Type> ProjectionMode;//Projection���: perspective, orthographic

    UPROPERTY(EditDefaultsOnly, Category = "Camera")
	float OrthoWidth = 2000.f;

    UPROPERTY(EditDefaultsOnly, Category = "Camera")
	float FieldOfView = 90.f;

    UPROPERTY(EditDefaultsOnly, Category = "Camera")
    float BlendIn = 0.f;//Playerī�޶󿡼� Warpī�޶�� ��ȯ�Ǵ� �ð�

    UPROPERTY(EditDefaultsOnly, Category = "Camera")
	float BlendOut = 0.f;//Warpī�޶󿡼� Playerī�޶�� ��ȯ�Ǵ� �ð�

    APlayerController* PlayerController;
    ACameraActor* CameraActor;
};

