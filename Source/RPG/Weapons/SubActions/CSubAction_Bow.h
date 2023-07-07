#pragma once

#include "CoreMinimal.h"
#include "Weapons/CSubAction.h"
#include "Components/TimelineComponent.h"
#include "CSubAction_Bow.generated.h"

USTRUCT()
struct FAimData
{
    GENERATED_BODY()

public:
    UPROPERTY(EditAnywhere)
        float TargetArmLength = 100;

    UPROPERTY(EditAnywhere)
        FVector SocketOffset = FVector(0, 30, 10);

    UPROPERTY(EditAnywhere)
        bool bEnableCameraLag;

    UPROPERTY(EditAnywhere)
        FVector CameraLocation;
};

UCLASS(Blueprintable)
class RPG_API UCSubAction_Bow : public UCSubAction
{
    GENERATED_BODY()

private:
    UPROPERTY(EditAnywhere, Category = "Aiming")
        class UCurveVector* Curve;

    UPROPERTY(EditAnywhere, Category = "Aiming")
        FAimData AimData;

    UPROPERTY(EditAnywhere, Category = "Aiming")
        float AimingSpeed = 200;

public:
    UCSubAction_Bow();

public:
    virtual void Pressed() override;
    virtual void Released() override;

public:
    void BeginPlay(class ACharacter* InOwner, class ACAttachment* InAttachment, class UCDoAction* InDoAction) override;

public:
    void Tick_Implementation(float InDeltaTime) override;

private:
    UFUNCTION()
        void onAiming(FVector Output);

private:
    class USpringArmComponent* SpringArm;
    class UCameraComponent* Camera;

private:
    FTimeline Timeline;
    //SubAction는 Actor가 아니기 때문에 Component를 가질 수 없다. SubAction은 UObject 상속이다.
    //그래서 TimelineComponent가 아닌 Timeline으로 작업한다.

private:
    FAimData OriginData;

private:
    float* Bend;
};