#include "Notifies/CAnimNotifyState_CameraAnim.h"
#include "Global.h"
#include "GameFramework/Character.h"
#include "GameFramework/PlayerController.h"//Player CameraManager�� ������ �ִ�.
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

	ACharacter* target = character;//Unreal Editor���� �������� ������ �⺻���� Player��.
	if (false == Target.IsNull())//Target�� �ִٸ�(=Unreal Editor���� Target�� �����ߴٸ�)
	{
		target = Target.Get();
	}

	FVector location = target->GetActorLocation();//Target�� ��ġ

	float yaw = controller->GetControlRotation().Yaw;
	location += FRotator(0., yaw, 0).RotateVector(LocationOffset);//LocationOffset���� yaw��ŭ ���� ���� ���Ѵ�.

	CheckNull(ModifierClass);//CameraModifier�� ���ٸ� ����.
	UCameraModifier* modifier = controller->PlayerCameraManager->AddNewCameraModifier(ModifierClass);
	Modifier = Cast<UCCameraModifier>(modifier);//CameraModifier �Ҵ�.

	Modifier->SetLocation(location);//��ġ ���� �־��ش�.
	Modifier->SetRotation(FRotator(PitchOffset, yaw, 0));//ȸ������ �־��ش�.

	controller->SetControlRotation(FRotator(0, controller->GetControlRotation().Yaw, 0));//controller�� Yaw���� ���������ش�.
	controller->PlayerCameraManager->PlayCameraAnim(CameraAnim, PlayRatio, 1, BlendInTime, BlendOutTime, false, false, 0, ECameraShakePlaySpace::UserDefined, controller->GetControlRotation());//UserDefined�� �츮���� ����(�츮�� Play�ϴ� �� ����), World�� ������ġ, CameraLocal�� relative��ġ
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

	controller->PlayerCameraManager->RemoveCameraModifier(Modifier);//���� �Ŀ� ����.
}
