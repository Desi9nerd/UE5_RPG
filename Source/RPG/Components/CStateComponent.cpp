#include "CStateComponent.h"
#include "Global.h"
#include "Characters/CAIController.h"
#include "Characters/CPlayer.h"

UCStateComponent::UCStateComponent()
{
	AController* controller = Cast<AController>(GetOwner());
	//OwnerCPlayer = Cast<ACPlayer>(controller->GetPawn());
}

void UCStateComponent::BeginPlay()
{
	Super::BeginPlay();

}

void UCStateComponent::SetIdleMode()
{
	ChangeType(EStateType::Idle);

	//CheckNull(OwnerCPlayer);
	//OwnerCPlayer->GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	UGameplayStatics::GetPlayerCharacter(this, 0)->GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
}

void UCStateComponent::SetDodgeMode()
{
	ChangeType(EStateType::Dodge);

	//CheckNull(OwnerCPlayer);
	//OwnerCPlayer->GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	UGameplayStatics::GetPlayerCharacter(this, 0)->GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
}

void UCStateComponent::SetEquipMode()
{
	ChangeType(EStateType::Equip);
}

void UCStateComponent::SetHittedMode()
{
	ChangeType(EStateType::Hitted);
}

void UCStateComponent::SetDeadMode()
{
	ChangeType(EStateType::Dead);
}

void UCStateComponent::SetActionMode()
{
	ChangeType(EStateType::Action);
}

void UCStateComponent::SetParryingMode()
{
	ChangeType(EStateType::Parrying);
}

void UCStateComponent::OnSubActionMode()
{
	bInSubActionMode = true;
}

void UCStateComponent::OffSubActionMode()
{
	bInSubActionMode = false;
}

void UCStateComponent::ChangeType(EStateType InType)
{
	EStateType prevType = Type;
	Type = InType;

	if (OnStateTypeChanged.IsBound())
		OnStateTypeChanged.Broadcast(prevType, Type);
}
