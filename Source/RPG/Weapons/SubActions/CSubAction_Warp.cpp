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
	//선택하지 않아도 기본자료형으로 만들 수 있도록 설정. Abstract이 아닌 경우 null 대신 많이 사용하는 형식이다.
	CameraActorClass = ACameraActor::StaticClass();
}

void UCSubAction_Warp::BeginPlay(ACharacter* InOwner, ACAttachment* InAttachment, UCDoAction* InDoAction)
{
	Super::BeginPlay(InOwner, InAttachment, InDoAction);

	PlayerController = InOwner->GetController<APlayerController>();//PlayController를 가져온다.

	CameraActor = InOwner->GetWorld()->SpawnActor<ACameraActor>(CameraActorClass);//CameraActor를 Spawn 시킨다.
	CameraActor->SetActorRotation(FRotator(-90.0f, 0.0f, 0.0f));//위에서 아래를 바라보도록 Rotator 설정.

	UCameraComponent* camera = CHelpers::GetComponent<UCameraComponent>(CameraActor);//CameraComponent 생성
	//변수로 설정한 값들을 camera의 해당 항목에 대입시켜 준다.
	camera->ProjectionMode = ProjectionMode;
	camera->OrthoWidth = OrthoWidth;
	camera->FieldOfView = FieldOfView;
}

void UCSubAction_Warp::Tick_Implementation(float InDeltaTime)
{
	Super::Tick_Implementation(InDeltaTime);

	CheckNull(PlayerController);
	CheckNull(CameraActor);

	//카메라의 위치를 Player위치로 실시간 옮겨준다. 
	CameraActor->SetActorLocation(Owner->GetActorLocation() + CameraRelativeLocation);
}

void UCSubAction_Warp::Pressed()
{
	CheckNull(PlayerController);
	CheckTrue(State->IsSubActionMode());

	Super::Pressed();


	State->OnSubActionMode();
	Movement->EnableTopViewCamera();//CMovementComponent의 bTopViewCamera를 true로 만듬. 절대방향으로 움직임.
	PlayerController->SetViewTargetWithBlend(CameraActor, BlendIn);
}

void UCSubAction_Warp::Released()
{
	CheckNull(PlayerController);
	CheckFalse(State->IsSubActionMode());

	Super::Released();


	State->OffSubActionMode();
	Movement->DisableTopViewCamera();//CMovementComponent의 bTopViewCamera를 false로 만듬.
	PlayerController->SetViewTargetWithBlend(Owner, BlendOut);
}
