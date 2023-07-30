#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "ICharacter.generated.h"

UINTERFACE(MinimalAPI) //직렬화를 위한 부분
class UICharacter : public UInterface
{
	GENERATED_BODY()
};

class RPG_API IICharacter
{
	GENERATED_BODY()

public:
	virtual void End_Dodge() { }
	virtual void End_Hitted() { }
	virtual void End_Dead() { }

public:
	void Create_DynamicMaterial(class ACharacter* InCharacter);
	void Change_Color(class ACharacter* InCharacter, FLinearColor InColor);
};


//추상클래스는 직렬화x, 인터페이스는 직렬화o(권장x)
