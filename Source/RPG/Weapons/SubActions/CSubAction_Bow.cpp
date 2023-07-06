#include "Weapons/SubActions/CSubAction_Bow.h"
#include "Global.h"
#include "GameFramework/Character.h"
#include "GameFramework/PlayerController.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "Components/CStateComponent.h"
#include "Weapons/Attachments/CAttachment_Bow.h"

UCSubAction_Bow::UCSubAction_Bow()
{
	CHelpers::GetAsset<UCurveVector>(&Curve, "CurveVector'/Game/Weapons/Bow/Curve_Aiming.Curve_Aiming'");//Editor에서 만든 CurveVector를 할당한다.
}

void UCSubAction_Bow::Pressed()
{
	CheckTrue(State->IsSubActionMode());
	CheckNull(SpringArm);
	CheckNull(Camera);


	Super::Pressed();

	State->OnSubActionMode();

	OriginData.TargetArmLength = SpringArm->TargetArmLength;
	OriginData.SocketOffset = SpringArm->SocketOffset;
	OriginData.bEnableCameraLag = SpringArm->bEnableCameraLag;
	OriginData.CameraLocation = Camera->GetRelativeLocation();

	SpringArm->TargetArmLength = AimData.TargetArmLength;
	SpringArm->SocketOffset = AimData.SocketOffset;
	SpringArm->bEnableCameraLag = AimData.bEnableCameraLag;
	Camera->SetRelativeLocation(AimData.CameraLocation);

	Timeline.PlayFromStart();//Timeline 동작 시작.
}

void UCSubAction_Bow::Released()
{
	CheckFalse(State->IsSubActionMode());
	CheckNull(SpringArm);
	CheckNull(Camera);


	Super::Released();

	State->OffSubActionMode();

	SpringArm->TargetArmLength = OriginData.TargetArmLength;
	SpringArm->SocketOffset = OriginData.SocketOffset;
	SpringArm->bEnableCameraLag = OriginData.bEnableCameraLag;
	Camera->SetRelativeLocation(OriginData.CameraLocation);

	Timeline.ReverseFromEnd();//Timeline 뒤집기
}

void UCSubAction_Bow::BeginPlay(ACharacter* InOwner, ACAttachment* InAttachment, UCDoAction* InDoAction)
{
	Super::BeginPlay(InOwner, InAttachment, InDoAction);

	SpringArm = CHelpers::GetComponent<USpringArmComponent>(InOwner);
	Camera = CHelpers::GetComponent<UCameraComponent>(InOwner);


	FOnTimelineVector timeline;
	timeline.BindUFunction(this, "OnAiming");//OnAiming함수를 묶어서 콜한다.

	Timeline.AddInterpVector(Curve, timeline);
	Timeline.SetPlayRate(AimingSpeed);
}

void UCSubAction_Bow::Tick_Implementation(float InDeltaTime)
{
	Super::Tick_Implementation(InDeltaTime);

	//TimelineComponent로 사용하는 경우 자기자신이 Tick이 있기 때문에 아래 작업을 안 해도 상관없으나 CSubAction_Bow의 경우 TimelineComponent가 없기 때문에 Tick을 반드시 넣어주어야 한다.
	Timeline.TickTimeline(InDeltaTime);
}

void UCSubAction_Bow::onAiming(FVector Output)
{
	Camera->FieldOfView = Output.X;//조준 활성화와 해제에 앞뒤 ZoomIn&Out에 Output.X값이 쓰인다.
}
