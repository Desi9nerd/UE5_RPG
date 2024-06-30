#pragma once
#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CAttachment.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FAttachmentBeginCollision);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FAttachmentEndCollision);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FAttachmentBeginOverlap, class ACharacter*, InAttacker, AActor*, InAttackCauser, class ACharacter*, InOther);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FAttachmentEndOverlap, class ACharacter*, InAttacker, class ACharacter*, InOther);

UCLASS()
class RPG_API ACAttachment : public AActor
{
	GENERATED_BODY()

public:
	ACAttachment();

	UFUNCTION(BlueprintNativeEvent)
	void OnBeginEquip();
	virtual void OnBeginEquip_Implementation() {}

	UFUNCTION(BlueprintNativeEvent)
	void OnUnequip();
	virtual void OnUnequip_Implementation() {}

	void OnCollisions(); //Collision �ѱ�
	void OffCollisions();//Collision ����

	UFUNCTION(BlueprintCallable, Category = "Attach")
	void AttachTo(FName InSocketName);
	UFUNCTION(BlueprintCallable, Category = "Attach")
	void AttachToCollision(FName InCollisionName);

	FAttachmentBeginCollision OnAttachmentBeginCollision;
	FAttachmentEndCollision OnAttachmentEndCollision;

	FAttachmentBeginOverlap OnAttachmentBeginOverlap;
	FAttachmentEndOverlap OnAttachmentEndOverlap;

	UPROPERTY()
	FHitResult HitResult_CAttachment;//ImpactPoint�� ���� �߰���

protected:
	virtual void BeginPlay() override;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
	USceneComponent* Root;

	UPROPERTY(BlueprintReadOnly, Category = "Game")
	ACharacter* OwnerCharacter;

	//UShapeComponent�� UBox,Capsule,SphereComponent�� ����Ŭ����
	UPROPERTY(BlueprintReadOnly, Category = "Game")
	TArray<UShapeComponent*> Collisions;

private:
	UFUNCTION()
	void OnComponentBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void OnComponentEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
};
