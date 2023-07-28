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

private://DataAsset을 받아온다.
	UPROPERTY(EditAnywhere, Category = "DataAsset")
		class UCWeaponAsset* DataAssets[(int32)EWeaponType::Max];

public:
	FORCEINLINE EWeaponType GetWeaponType() { return Type; }

public: //무기 Type이 맞는지 확인해주는 함수들
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
	bool IsIdleMode();//StateComponent, WeaponComponent 둘 다 같은 레벨이다. 서로 소유할 수 없는 관계이기 때문에 참조만해서 리턴받기 위해 IsIdleMode를 사용한다.

public:
	class ACAttachment* GetAttachment();
	class UCEquipment* GetEquipment();
	class UCDoAction* GetDoAction();
	class UCSubAction* GetSubAction();

public:
	//무기 세팅
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

public: //무기가 바뀌었을때 통보해줄 delegate
	FWeaponTypeChanged OnWeaponTypeChange;

private:
	class ACharacter* OwnerCharacter;

	EWeaponType Type = EWeaponType::Max;

private:
	//가비지 콜랙터가 삭제하지 않도록 UPROPERTY를 붙여 직렬화 시켜준다. 직렬화되면 가비지 콜렉터가 삭제하지 않는다. UPROPERTY가 없으면 터진다.
	UPROPERTY()
		class UCWeaponData* Datas[(int32)EWeaponType::Max];//실제로 생성된 데이터

public:
	TArray<ACItem*> ItemsArray;
};