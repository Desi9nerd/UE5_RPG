#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Item/CItem.h"
#include "CAttachment.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FAttachmentBeginCollision);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FAttachmentEndCollision);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FAttachmentBeginOverlap, class ACharacter*, InAttacker, AActor*, InAttackCauser, class ACharacter*, InOther);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FAttachmentEndOverlap, class ACharacter*, InAttacker, class ACharacter*, InOther);

//공중콤보 Delegate, 사용할지 결정 후 지금 주석 지우기
//DECLARE_DYNAMIC_MULTICAST_DELEGATE(FAttachmentBeginCollision_AirCombo);
//DECLARE_DYNAMIC_MULTICAST_DELEGATE(FAttachmentEndCollision_AirCombo);
//
//DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams/(FAttachmentBeginOverlap_AirCombo,/ class ACharacter*, InAttacker, AActor*, /InAttackCauser, class ACharacter*, /InOther);
//DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FAttachmentEndOverlap_AirCombo, class ACharacter*, InAttacker, class ACharacter*, InOther);

UCLASS()
class RPG_API ACAttachment : public ACItem
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
	void OnCollisions(); //Collision 켜기
	void OffCollisions();//Collision 끄기

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


	//공중콤보 Delegate, 사용할지 결정 후 지금 주석 지우기
	//FAttachmentBeginCollision_AirCombo OnAttachmentBeginCollision_AirCombo;
	//FAttachmentEndCollision_AirCombo OnAttachmentEndCollision_AirCombo;
	//
	//FAttachmentBeginOverlap_AirCombo OnAttachmentBeginOverlap_AirCombo;
	//FAttachmentEndOverlap_AirCombo OnAttachmentEndOverlap_AirCombo;

protected:
	UPROPERTY(BlueprintReadOnly, Category = "Game")
		class ACharacter* OwnerCharacter;

	//UShapeComponent는 UBox,Capsule,SphereComponent의 상위클래스
	UPROPERTY(BlueprintReadOnly, Category = "Game")
		TArray<class UShapeComponent*> Collisions;

/** Pick Up
 * ////////////////////////////////////////////////////////
 */ 

public:
	FORCEINLINE FName GetHolsterSocketName() { return HolsterSocketName; }
	
public:
	void Equip(USceneComponent* InParent, FName InSocketName, AActor* NewOwner, APawn* NewInstigator);
	void PlayEquipSound();
	void DisableSphereCollision();
	void DeactivateEmbers();

private:
	UPROPERTY(EditAnywhere, Category = "PickUp")
	USoundBase* EquipSound;

protected:
	UPROPERTY(EditAnywhere, Category = "PickUp")
	FName HolsterSocketName;

	//UPROPERTY(EditAnywhere, Category = "PickUp")
	//FVector SpawnLocation;
};
