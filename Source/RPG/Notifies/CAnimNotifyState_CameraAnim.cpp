#include "Notifies/CAnimNotifyState_CameraAnim.h"
#include "Global.h"
#include "GameFramework/Character.h"
#include "GameFramework/PlayerController.h"//Player CameraManager를 가지고 있다.
#include "Camera/PlayerCameraManager.h"
#include "Camera/CameraAnim.h"
#include "Weapons/AddOns/CCameraModifier.h"

UCAnimNotifyState_CameraAnim::UCAnimNotifyState_CameraAnim()
{
	ModifierClass = UCCameraModifier::StaticClass();
}

FString UCAnimNotifyState_CameraAnim::GetNotifyName_Implementation() const
{
	return "CameraAnim";
}

void UCAnimNotifyState_CameraAnim::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyBegin(MeshComp, Animation, TotalDuration, EventReference);

	CheckNull(MeshComp);
	CheckNull(MeshComp->GetOwner());
	CheckNull(CameraAnim);

	ACharacter* character = Cast<ACharacter>(MeshComp->GetOwner());
	CheckNull(character);

	APlayerController* controller = character->GetController<APlayerController>();
	CheckNull(controller);

	ACharacter* target = character;//Unreal Editor에서 설정하지 않으면 기본값은 Player다.
	if (false == Target.IsNull())//Target이 있다면(=Unreal Editor에서 Target을 지정했다면)
	{
		target = Target.Get();
	}

	FVector location = target->GetActorLocation();//Target의 위치

	float yaw = controller->GetControlRotation().Yaw;
	location += FRotator(0., yaw, 0).RotateVector(LocationOffset);//LocationOffset에서 yaw만큼 돌린 값을 더한다.

	CheckNull(ModifierClass);//CameraModifier가 없다면 리턴.
	UCameraModifier* modifier = controller->PlayerCameraManager->AddNewCameraModifier(ModifierClass);
	Modifier = Cast<UCCameraModifier>(modifier);//CameraModifier 할당.

	Modifier->SetLocation(location);//위치 값을 넣어준다.
	Modifier->SetRotation(FRotator(PitchOffset, yaw, 0));//회전값을 넣어준다.

	controller->SetControlRotation(FRotator(0, controller->GetControlRotation().Yaw, 0));//controller의 Yaw값을 고정시켜준다.
	controller->PlayerCameraManager->PlayCameraAnim(CameraAnim, PlayRatio, 1, BlendInTime, BlendOutTime, false, false, 0, ECameraShakePlaySpace::UserDefined, controller->GetControlRotation());//UserDefined는 우리공간 기준(우리가 Play하는 곳 기점), World는 월드위치, CameraLocal은 relative위치
}

void UCAnimNotifyState_CameraAnim::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyEnd(MeshComp, Animation, EventReference);

	CheckNull(MeshComp);
	CheckNull(MeshComp->GetOwner());
	CheckNull(CameraAnim);

	ACharacter* character = Cast<ACharacter>(MeshComp->GetOwner());
	CheckNull(character);

	APlayerController* controller = character->GetController<APlayerController>();
	CheckNull(controller);

	controller->PlayerCameraManager->RemoveCameraModifier(Modifier);//끝난 후에 해제.
}
