#include "CAnimNotify_EndState.h"
#include "Global.h"
#include "Characters/ICharacter.h"

FString UCAnimNotify_EndState::GetNotifyName_Implementation() const
{
	return "EndState";
}

void UCAnimNotify_EndState::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	Super::Notify(MeshComp, Animation);
	CheckNull(MeshComp);
	CheckNull(MeshComp->GetOwner());

	IICharacter* character = Cast<IICharacter>(MeshComp->GetOwner());
	CheckNull(character);

	switch(StateType)
	{
		//Dead를 Hitted보다 먼저 검사할 수 있게 순서를 맞춰준다.
		case EStateType::Dodge: character->End_Dodge(); break;
		case EStateType::Dead: character->End_Dead(); break;
		case EStateType::Hitted: character->End_Hitted(); break;
	}
}