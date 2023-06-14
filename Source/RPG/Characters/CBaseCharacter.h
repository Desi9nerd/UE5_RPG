#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "CBaseCharacter.generated.h"

UCLASS()
class RPG_API ACBaseCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	ACBaseCharacter();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

};
