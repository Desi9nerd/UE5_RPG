#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
//#include "Item/CItem.h"
#include "CAttachment.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FAttachmentBeginCollision);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FAttachmentEndCollision);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FAttachmentBeginOverlap, class ACharacter*, InAttacker, AActor*, InAttackCauser, class ACharacter*, InOther);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FAttachmentEndOverlap, class ACharacter*, InAttacker, class ACharacter*, InOther);


UCLASS()
class RPG_API ACAttachment : public AActor
{
	GENERATED_BODY()

protected:
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
		class USceneComponent* Root;
	
public:	
	ACAttachment();

protected:
	virtual void BeginPlay() override;

public:
	UFUNCTION(BlueprintNativeEvent)
		void OnBeginEquip();
	virtual void OnBeginEquip_Implementation() {}

	UFUNCTION(BlueprintNativeEvent)
		void OnUnequip();
	virtual void OnUnequip_Implementation() {}

public:
	void OnCollisions(); //Collision �ѱ�
	void OffCollisions();//Collision ����

private:
	UFUNCTION()
		void OnComponentBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
		void OnComponentEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
	
public:
	UFUNCTION(BlueprintCallable, Category = "Attach")
		void AttachTo(FName InSocketName);

	UFUNCTION(BlueprintCallable, Category = "Attach")
		void AttachToCollision(FName InCollisionName);

public:
	FAttachmentBeginCollision OnAttachmentBeginCollision;
	FAttachmentEndCollision OnAttachmentEndCollision;

	FAttachmentBeginOverlap OnAttachmentBeginOverlap;
	FAttachmentEndOverlap OnAttachmentEndOverlap;

protected:
	UPROPERTY(BlueprintReadOnly, Category = "Game")
		class ACharacter* OwnerCharacter;

	//UShapeComponent�� UBox,Capsule,SphereComponent�� ����Ŭ����
	UPROPERTY(BlueprintReadOnly, Category = "Game")
		TArray<class UShapeComponent*> Collisions;


public:
	UPROPERTY() //2023.08.07 ImpactPoint�� ���� �߰���
	FHitResult HitResult_CAttachment;

/** Pick Up
 * ////////////////////////////////////////////////////////
 */ 

public:
	FORCEINLINE FName GetHolsterSocketName() { return HolsterSocketName; }
	
public:
	//void Equip(USceneComponent* InParent, FName /InSocketName, /AActor* NewOwner, APawn* NewInstigator);
	//void PlayEquipSound();
	//void DisableSphereCollision();
	//void DeactivateEmbers();

private:
	UPROPERTY(EditAnywhere, Category = "PickUp")
	USoundBase* EquipSound;

protected:
	UPROPERTY(EditAnywhere, Category = "PickUp")
	FName HolsterSocketName;

	//UPROPERTY(EditAnywhere, Category = "PickUp")
	//FVector SpawnLocation;
};
