#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "CWeaponComponent.generated.h"

class ACItem;

UENUM(BlueprintType)
enum class EWeaponType : uint8
{
	Fist, Sword, Hammer, Warp, Around, Bow, Blade, Max,
};
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FWeaponTypeChanged, EWeaponType, InPrevType, EWeaponType, InNewType);

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class RPG_API UCWeaponComponent : public UActorComponent
{
	GENERATED_BODY()

private://DataAsset�� �޾ƿ´�.
	UPROPERTY(EditAnywhere, Category = "DataAsset")
		class UCWeaponAsset* DataAssets[(int32)EWeaponType::Max];

public:
	FORCEINLINE EWeaponType GetWeaponType() { return Type; }

public: //���� Type�� �´��� Ȯ�����ִ� �Լ���
	FORCEINLINE bool IsUnarmedMode() { return Type == EWeaponType::Max; }
	FORCEINLINE bool IsFistMode() { return Type == EWeaponType::Fist; }
	FORCEINLINE bool IsSwordMode() { return Type == EWeaponType::Sword; }
	FORCEINLINE bool IsHammerMode() { return Type == EWeaponType::Hammer; }
	FORCEINLINE bool IsWarpMode() { return Type == EWeaponType::Warp; }
	FORCEINLINE bool IsAroundMode() { return Type == EWeaponType::Around; }
	FORCEINLINE bool IsBowMode() { return Type == EWeaponType::Bow; }
	FORCEINLINE bool IsBladeMode() { return Type == EWeaponType::Bow; }

public:
	UCWeaponComponent();

protected:
	virtual void BeginPlay() override;

public:
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

private:
	bool IsIdleMode();//StateComponent, WeaponComponent �� �� ���� �����̴�. ���� ������ �� ���� �����̱� ������ �������ؼ� ���Ϲޱ� ���� IsIdleMode�� ����Ѵ�.

public:
	class ACAttachment* GetAttachment();
	class UCEquipment* GetEquipment();
	class UCDoAction* GetDoAction();
	class UCSubAction* GetSubAction();

public:
	//���� ����
	void SetUnarmedMode();
	void SetFistMode();
	void SetSwordMode();
	void SetHammerMode();
	void SetWarpMode();
	void SetAroundMode();
	void SetBowMode();
	void SetBladeMode();

	void DoAction();
	void SubAction_Pressed();
	void SubAction_Released();

private:
	void SetMode(EWeaponType InType);
	void ChangeType(EWeaponType InType);

public: //���Ⱑ �ٲ������ �뺸���� delegate
	FWeaponTypeChanged OnWeaponTypeChange;

private:
	class ACharacter* OwnerCharacter;

	EWeaponType Type = EWeaponType::Max;

private:
	//������ �ݷ��Ͱ� �������� �ʵ��� UPROPERTY�� �ٿ� ����ȭ �����ش�. ����ȭ�Ǹ� ������ �ݷ��Ͱ� �������� �ʴ´�. UPROPERTY�� ������ ������.
	UPROPERTY()
		class UCWeaponData* Datas[(int32)EWeaponType::Max];//������ ������ ������

public:
	TArray<ACItem*> ItemsArray;
};