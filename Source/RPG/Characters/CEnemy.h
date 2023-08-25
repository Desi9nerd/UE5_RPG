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

protected:
	virtual void BeginPlay() override;

public:
	void OnStateTypeChanged(EStateType InPrevType, EStateType InNewType) override;

protected:
	virtual void Hitted() override;//CBaseCharacter�� Hitted ������

public:
	virtual void End_Hitted() override;//CBaseCharacter�� End_Hitted ������

};