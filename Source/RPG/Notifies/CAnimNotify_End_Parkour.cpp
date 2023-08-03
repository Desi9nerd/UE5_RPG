#include "Notifies/CAnimNotify_End_Parkour.h"
#include "Global.h"
#include "Parkour/CParkourComponent.h"

FString UCAnimNotify_End_Parkour::GetNotifyName_Implementation() const
{
	return "Parkour";
}

void UCAnimNotify_End_Parkour::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
	const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation, EventReference);

	CheckNull(MeshComp);
	CheckNull(MeshComp->GetOwner());

	UCParkourComponent* parkour = CHelpers::GetComponent<UCParkourComponent>(MeshComp->GetOwner());
	CheckNull(parkour);

	parkour->End_DoParkour();//End_DoParkour를 콜해 파쿠르를 끝낸다.
}