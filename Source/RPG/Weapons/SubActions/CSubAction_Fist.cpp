#include "Weapons/SubActions/CSubAction_Fist.h"
#include "Global.h"
#include "GameFramework/Character.h"
#include "Components/CStateComponent.h"
#include "Components/CMovementComponent.h"
#include "Weapons/CAttachment.h"
#include "Weapons/CDoAction.h"
#include "Weapons/AddOns/CGhostTrail.h"

void UCSubAction_Fist::Pressed()
{
	CheckFalse(State->IsIdleMode());
	CheckTrue(State->IsSubActionMode());

	Super::Pressed();

	State->SetActionMode();
	State->OnSubActionMode();

	GhostTrail = CHelpers::Play_GhostTrail(GhostTrailClass, Owner);//GhostTrail �÷���.

	ActionData.DoAction(Owner);
}

void UCSubAction_Fist::Begin_SubAction_Implementation()
{
	Super::Begin_SubAction_Implementation();

	//DoAction�� �浹 �̺�Ʈ ���� ����(=DELEGATE ���� ����)
	Attachment->OnAttachmentEndCollision.Remove(DoAction, "OnAttachmentEndCollision");
	Attachment->OnAttachmentBeginOverlap.Remove(DoAction, "OnAttachmentBeginOverlap");

	//SubAction�� �浹 �̺�Ʈ ����(=DELEGATE ����)
	Attachment->OnAttachmentEndCollision.AddDynamic(this, &UCSubAction_Fist::OnAttachmentEndCollision);
	Attachment->OnAttachmentBeginOverlap.AddDynamic(this, &UCSubAction_Fist::OnAttachmentBeginOverlap);
}

void UCSubAction_Fist::End_SubAction_Implementation()
{
	Super::End_SubAction_Implementation();

	//SubAction�� �浹 �̺�Ʈ ���� ����(=DELEGATE ���� ����)
	Attachment->OnAttachmentEndCollision.Remove(this, "OnAttachmentEndCollision");
	Attachment->OnAttachmentBeginOverlap.Remove(this, "OnAttachmentBeginOverlap");

	//DoAction�� �浹 �̺�Ʈ �ٽ� ����(=DELEGATE ����)
	Attachment->OnAttachmentEndCollision.AddDynamic(DoAction, &UCDoAction::OnAttachmentEndCollision);
	Attachment->OnAttachmentBeginOverlap.AddDynamic(DoAction, &UCDoAction::OnAttachmentBeginOverlap);

	//���� ���·� �����ش�.
	State->SetIdleMode();
	State->OffSubActionMode();

	Movement->Move();
	Movement->DisableFixedCamera();

	GhostTrail->Destroy();//GhostTrail�� �����ش�.

	HitIndex = 0;//HitIndex �ʱ�ȭ
}

void UCSubAction_Fist::OnAttachmentBeginOverlap(ACharacter* InAttacker, AActor* InAttackCauser, ACharacter* InOther)
{
	CheckNull(InOther);

	for (ACharacter* character : Hitted)
		CheckTrue(character == InOther);

	Hitted.AddUnique(InOther);

	CheckTrue(HitIndex >= HitDatas.Num());
	HitDatas[HitIndex].SendDamage(Owner, InAttackCauser, InOther);
}

void UCSubAction_Fist::OnAttachmentEndCollision()
{
	Hitted.Empty();//����� ���� Hit Data�� ����ش�.

	HitIndex++;//���� Hit Montage�� �������� �ε����� ī�����Ͽ� ���� Hit Data�� �Ѿ��.
}
