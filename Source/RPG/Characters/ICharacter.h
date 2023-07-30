#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "ICharacter.generated.h"

UINTERFACE(MinimalAPI) //����ȭ�� ���� �κ�
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


//�߻�Ŭ������ ����ȭx, �������̽��� ����ȭo(����x)
