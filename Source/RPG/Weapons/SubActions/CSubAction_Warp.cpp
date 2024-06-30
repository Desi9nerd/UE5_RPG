#include "Weapons/SubActions/CSubAction_Warp.h"
#include "Global.h"
#include "GameFramework/Character.h"
#include "GameFramework/PlayerController.h"
#include "Components/CStateComponent.h"
#include "Components/CMovementComponent.h"
#include "Camera/CameraActor.h"
#include "Camera/CameraComponent.h"

UCSubAction_Warp::UCSubAction_Warp()
{
	//�������� �ʾƵ� �⺻�ڷ������� ���� �� �ֵ��� ����. Abstract�� �ƴ� ��� null ��� ���� ����ϴ� �����̴�.
	CameraActorClass = ACameraActor::StaticClass();
}

void UCSubAction_Warp::BeginPlay(ACharacter* InOwner, ACAttachment* InAttachment, UCDoAction* InDoAction)
{
	Super::BeginPlay(InOwner, InAttachment, InDoAction);

	PlayerController = InOwner->GetController<APlayerController>();//PlayController�� �����´�.

	CameraActor = InOwner->GetWorld()->SpawnActor<ACameraActor>(CameraActorClass);//CameraActor�� Spawn ��Ų��.
	CameraActor->SetActorRotation(FRotator(-90.0f, 0.0f, 0.0f));//������ �Ʒ��� �ٶ󺸵��� Rotator ����.

	UCameraComponent* camera = CHelpers::GetComponent<UCameraComponent>(CameraActor);//CameraComponent ����
	//������ ������ ������ camera�� �ش� �׸� ���Խ��� �ش�.
	camera->ProjectionMode = ProjectionMode;
	camera->OrthoWidth = OrthoWidth;
	camera->FieldOfView = FieldOfView;
}

void UCSubAction_Warp::Tick_Implementation(float InDeltaTime)
{
	Super::Tick_Implementation(InDeltaTime);

	CheckNull(PlayerController);
	CheckNull(CameraActor);

	//ī�޶��� ��ġ�� Player��ġ�� �ǽð� �Ű��ش�. 
	CameraActor->SetActorLocation(Owner->GetActorLocation() + CameraRelativeLocation);
}

void UCSubAction_Warp::Pressed()
{
	CheckNull(PlayerController);
	CheckTrue(State->IsSubActionMode());

	Super::Pressed();


	State->OnSubActionMode();
	Movement->EnableTopViewCamera();//CMovementComponent�� bTopViewCamera�� true�� ����. ����������� ������.
	PlayerController->SetViewTargetWithBlend(CameraActor, BlendIn);
}

void UCSubAction_Warp::Released()
{
	CheckNull(PlayerController);
	CheckFalse(State->IsSubActionMode());

	Super::Released();


	State->OffSubActionMode();
	Movement->DisableTopViewCamera();//CMovementComponent�� bTopViewCamera�� false�� ����.
	PlayerController->SetViewTargetWithBlend(Owner, BlendOut);
}
