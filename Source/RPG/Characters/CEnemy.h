#pragma once
#include "CoreMinimal.h"
#include "Characters/ECharacterTypes.h"
#include "Characters/CBaseCharacter.h"
#include "CEnemy.generated.h"

UCLASS()
class RPG_API ACEnemy
	: public ACBaseCharacter
{
	GENERATED_BODY()

public:
	ACEnemy();
	void OnStateTypeChanged(EStateType InPrevType, EStateType InNewType) override;
	virtual void End_Hitted() override;//CBaseCharacter의 End_Hitted 재정의

protected:
	virtual void BeginPlay() override;
	virtual void Hitted() override;//CBaseCharacter의 Hitted 재정의
};