#pragma once

//TODO:
//StateComponent Enum 옮기기
//StatusComponent Enum 옮기기

//TODO:
//ActionMode 기준
//if(State->IsAction())
//WeaponType과 HitReactionType을 조합하여
//Enemy 기준에서 HitData의 Attacker는 Player, Causer는 무기이다. 
//Enemy가 HitData에서 Causer인 무기를 판단하여 HitReactionType을 결정해 다른 몽타주가 나오도록 해보자.
//TODO
//SubActionMode 기준
//if(State->IsSubActionMode())
//Player가 SubActionMode일 때 위와 같은 방법으로 구현.

UENUM() //CStateComponent.h에 있던것 옮김
enum class EStateType : uint8
{
	Idle = 0, Dodge, Equip, Hitted, Dead, Action, Parrying, Max,
};



//////////////////////////////////////////////////////////

UENUM(BlueprintType) //BP에 리플렉션 해준다.
enum class ECharacterState : uint8
{
	ECS_Unequipped UMETA(DisplayName = "Unequipped"),
	ECS_EquippedOneHandedWeapon UMETA(DisplayName = "Equipped One-Handed Weapon"),
	ECS_EquippedTwoHandedWeapon UMETA(DisplayName = "Equipped Two-Handed Weapon")
};

UENUM(BlueprintType) //BP에 리플렉션 해준다.
enum class EActionState : uint8
{
	EAS_Unoccupied UMETA(DisplayName = "Unocuppied"),
	EAS_HitReaction UMETA(DisplayName = "HitReaction"),
	EAS_Attacking UMETA(DisplayName = "Attacking"),
	EAS_EquippingWeapon UMETA(DisplayName = "Equipping Weapon"),
	EAS_Dodge UMETA(DisplayName = "Dodge"),
	EAS_Dead UMETA(DisplayName = "Dead")
};

UENUM(BlueprintType) //BP에 리플렉션 해준다.
enum EDeathPose
{
	EDP_Death1 UMETA(DisplayName = "Death1"),
	EDP_Death2 UMETA(DisplayName = "Death2"),
	EDP_Death3 UMETA(DisplayName = "Death3"),
	EDP_Death4 UMETA(DisplayName = "Death4"),
	EDP_Death5 UMETA(DisplayName = "Death5"),
	EDP_Death6 UMETA(DisplayName = "Death6"),

	EDP_MAX UMETA(DisplayName = "DefaultMAX")
};

UENUM(BlueprintType) //BP에 리플렉션 해준다.
enum class EEnemyState : uint8
{
	EES_NoState UMETA(DisplayName = "NoState"),

	EES_Dead UMETA(DisplayName = "Dead"),
	EES_Patrolling UMETA(DisplayName = "Patrolling"),
	EES_Chasing UMETA(DisplayName = "Chasing"),
	EES_Attacking UMETA(DisplayName = "Attacking"),
	EES_Engaged UMETA(DisplayName = "Engaged")
};
