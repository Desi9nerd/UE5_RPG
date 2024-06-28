#pragma once
#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "CAnimInstance_Bow.generated.h"

UCLASS()
class RPG_API UCAnimInstance_Bow : public UAnimInstance
{
	GENERATED_BODY()

public:
	//�����ͷ� �ܺο��� ������ ������ ���� �ǽð����� �ٲ��� �� �ִ�.  
	FORCEINLINE float* GetBend() { return &Bend; }

	void NativeBeginPlay() override;
	void NativeUpdateAnimation(float DeltaSeconds) override;

protected:
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Animation")
	float Bend;//Ȱ ������ ���η����� ����

};
