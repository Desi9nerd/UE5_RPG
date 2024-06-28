#pragma once
#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CGhostTrail.generated.h"

class UPoseableMeshComponent;
class ACharacter;
class UMaterialInstanceDynamic;

UCLASS(Blueprintable)
class RPG_API ACGhostTrail : public AActor
{
	GENERATED_BODY()
		
public:
	ACGhostTrail();

protected:
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

private:
	void OnUpdateGhostTrail();

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

	UPROPERTY(VisibleDefaultsOnly)
	UPoseableMeshComponent* Mesh;

	ACharacter* Owner;
	UMaterialInstanceDynamic* Material;

	FTimerHandle TimerHandle;//Ÿ�̸� ������ ���� ����.
};
