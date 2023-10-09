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

public:
    UCSubAction_Bow();
    void BeginPlay(class ACharacter* InOwner, class ACAttachment* InAttachment, class UCDoAction* InDoAction) override;
    void Tick_Implementation(float InDeltaTime) override;
    virtual void Pressed() override;
    virtual void Released() override;

private:
    UFUNCTION()
	void OnAiming(FVector Output);

private:
    UPROPERTY(EditAnywhere, Category = "Aiming")
        class UCurveVector* Curve;

    UPROPERTY(EditAnywhere, Category = "Aiming")
        FAimData AimData;

    UPROPERTY(EditAnywhere, Category = "Aiming")
        float AimingSpeed = 200;

    class USpringArmComponent* SpringArm;
    class UCameraComponent* Camera;
    //SubAction�� Actor�� �ƴϱ� ������ Component�� ���� �� ����. SubAction�� UObject ����̴�.
    //�׷��� TimelineComponent�� �ƴ� Timeline���� �۾��Ѵ�.
    FTimeline Timeline;
    FAimData OriginData;
    float* Bend;
};