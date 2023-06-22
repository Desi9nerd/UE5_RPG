#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CGhostTrail.generated.h"

UCLASS()
class RPG_API ACGhostTrail : public AActor
{
	GENERATED_BODY()

private:
	UPROPERTY(EditDefaultsOnly, Category = "Capture")
		float StartDelay = 0;//Ű�� ���� �� �� �� ���� �Ŀ� �÷����� �� ���� ����

	UPROPERTY(EditDefaultsOnly, Category = "Capture")
		float Interval = 0.25f;//�ִϸ��̼��� ĸó�Ǵ� ����

	UPROPERTY(EditDefaultsOnly, Category = "Capture")
		FLinearColor Color = FLinearColor(1, 1, 1, 1);

	UPROPERTY(EditDefaultsOnly, Category = "Capture")
		float Exponent = 1;//�����ð��� ���������� ������ ��

	UPROPERTY(EditDefaultsOnly, Category = "Capture")
		FVector Scale = FVector::OneVector;//GhostTrail ũ��

	UPROPERTY(EditDefaultsOnly, Category = "Capture")
		FVector ScaleAmount = FVector::ZeroVector;//GhostTrail ��ġ
private:
	UPROPERTY(VisibleDefaultsOnly)
		class UPoseableMeshComponent* Mesh;

public:
	ACGhostTrail();

protected:
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

private:
	class ACharacter* Owner;
	class UMaterialInstanceDynamic* Material;

	FTimerHandle TimerHandle;//Ÿ�̸� ������ ���� ����.
};
