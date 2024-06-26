#pragma once
#include "CoreMinimal.h"
#include "Weapons/CSubAction.h"
#include "Weapons/CWeaponStructures.h"
#include "CSubAction_Fist.generated.h"

UCLASS(Blueprintable)//블루프린트화해서 설정할 수 있도록 Blueprintable 명시
class RPG_API UCSubAction_Fist : public UCSubAction
{
	GENERATED_BODY()

public:
	void Pressed() override;

	void Begin_SubAction_Implementation() override;
	void End_SubAction_Implementation() override;

	UPROPERTY(EditAnywhere, Category = "Action")
	FDoActionData ActionData;

	UPROPERTY(EditAnywhere, Category = "Action")
	TArray<FHitData> HitDatas;

	UPROPERTY(EditAnywhere, Category = "Action")
	TSubclassOf<class ACGhostTrail> GhostTrailClass;

private:
	//********************************************************
	//**충돌 처리를 위한 함수
	UFUNCTION()
	void OnAttachmentEndCollision();

	UFUNCTION()
	void OnAttachmentBeginOverlap(class ACharacter* InAttacker, AActor* InAttackCauser, class ACharacter* InOther);
	//********************************************************

	TArray<ACharacter*> Hitted;
	int32 HitIndex;
	
	ACGhostTrail* GhostTrail;
};
