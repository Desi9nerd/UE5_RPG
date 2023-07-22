#include "Weapons/SubActions/CSubAction_Bow.h"
#include "Global.h"
#include "GameFramework/Character.h"
#include "GameFramework/PlayerController.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "Components/CStateComponent.h"
#include "Components/WidgetComponent.h"
#include "Weapons/Attachments/CAttachment_Bow.h"

UCSubAction_Bow::UCSubAction_Bow()
{
	CHelpers::GetAsset<UCurveVector>(&Curve, "CurveVector'/Game/Weapons/Bow/Curve_Aiming.Curve_Aiming'");//Editor���� ���� CurveVector�� �Ҵ��Ѵ�.


	//CrossHair Widget ����
	//CrossHair = CreateWidget<UWidgetComponent>(this, UCUserWidget_CrossHair::StaticClass(), "CrossHair");

	CHelpers::GetClass<UCUserWidget_CrossHair>(&crosshairClass, "WidgetBlueprint'/Game/Widgets/WB_CrossHair.WB_CrossHair_C'");

	//CrossHair->SetWidgetClass(crosshairClass);

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

	Timeline.PlayFromStart();//Timeline ���� ����.
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

	Timeline.ReverseFromEnd();//Timeline ������
}

void UCSubAction_Bow::BeginPlay(ACharacter* InOwner, ACAttachment* InAttachment, UCDoAction* InDoAction)
{
	Super::BeginPlay(InOwner, InAttachment, InDoAction);

	SpringArm = CHelpers::GetComponent<USpringArmComponent>(InOwner);
	Camera = CHelpers::GetComponent<UCameraComponent>(InOwner);


	FOnTimelineVector timeline;
	timeline.BindUFunction(this, "OnAiming");//OnAiming�Լ��� ��� ���Ѵ�.

	Timeline.AddInterpVector(Curve, timeline);
	Timeline.SetPlayRate(AimingSpeed);

	ACAttachment_Bow* bow = Cast<ACAttachment_Bow>(InAttachment);//CAttachment_Bow�� ����� ����ϱ� ���� ĳ�����Ѵ�.

	if (!!bow)
		Bend = bow->GetBend();//CSubAction_Bow���� ������ Bend ������ CAttachment_Bow�� Bend���� GetBend()�� �����ͼ� �־��ش�.

	//CrossHair Widget ����
	
}

void UCSubAction_Bow::Tick_Implementation(float InDeltaTime)
{
	Super::Tick_Implementation(InDeltaTime);

	//TimelineComponent�� ����ϴ� ��� �ڱ��ڽ��� Tick�� �ֱ� ������ �Ʒ� �۾��� �� �ص� ��������� CSubAction_Bow�� ��� TimelineComponent�� ���� ������ Tick�� �ݵ�� �־��־�� �Ѵ�.
	Timeline.TickTimeline(InDeltaTime);
}

void UCSubAction_Bow::OnAiming(FVector Output)
{
	Camera->FieldOfView = Output.X;//���� Ȱ��ȭ�� ������ �յ� ZoomIn&Out�� Output.X���� ���δ�.

	if (!!Bend)
		*Bend = Output.Y;//�������� Curve_Aiming���� ������ Y��. 0~100���� ���´�.
}
