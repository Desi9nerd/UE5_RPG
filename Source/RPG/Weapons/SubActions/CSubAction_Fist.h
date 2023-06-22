#pragma once

#include "CoreMinimal.h"
#include "Weapons/CSubAction.h"
#include "Weapons/CWeaponStructures.h"
#include "CSubAction_Fist.generated.h"

UCLASS(Blueprintable)//�������Ʈȭ�ؼ� ������ �� �ֵ��� Blueprintable ���
class RPG_API UCSubAction_Fist : public UCSubAction
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, Category = "Action")
		FDoActionData ActionData;

	UPROPERTY(EditAnywhere, Category = "Action")
		TArray<FHitData> HitDatas;

	UPROPERTY(EditAnywhere, Category = "Action")
		TSubclassOf<class ACGhostTrail> GhostTrailClass;

public:
	void Pressed() override;

	void End_SubAction_Implementation() override;

private:
	TArray<class ACharacter*> Hitted;
	int32 HitIndex;

private:
	class ACGhostTrail* GhostTrail;
};
