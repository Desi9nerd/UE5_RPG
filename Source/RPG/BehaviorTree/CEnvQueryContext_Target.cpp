#include "BehaviorTree/CEnvQueryContext_Target.h"
#include "Global.h"
#include "EnvironmentQuery/EnvQueryTypes.h"
#include "EnvironmentQuery/Items/EnvQueryItemType_Actor.h"
#include "Characters/CEnemy_AI.h"
#include "Characters/CAIController.h"
#include "BehaviorTree/BlackboardComponent.h"

void UCEnvQueryContext_Target::ProvideContext(FEnvQueryInstance& QueryInstance, FEnvQueryContextData& ContextData) const
{
	Super::ProvideContext(QueryInstance, ContextData);
	
	//Query를 수행하는 Actor인 ACEnemy_AI 캐스팅
	ACEnemy_AI* ai = Cast<ACEnemy_AI>(QueryInstance.Owner.Get());//QueryInstance.Owner.Get()는 UObject 리턴이다. Querier가 Object, Actor, Location 등이 될 수 있으니 어느것이든 다 받을수 있도록 최상위형인 UObject로 받기 위해QueryInstance.Owner.Get()으로 받았다.
	ACAIController* controller = ai->GetController<ACAIController>();//Blackboard를 가져오기 위해 먼저 controller를 캐스팅한다.
	UBlackboardComponent* blackboard = controller->GetBlackboardComponent();//controller 내의 Blackboard를 가져온다.
	AActor* target = Cast<AActor>(blackboard->GetValueAsObject("Target"));//Blackboard의 Target를 가져다쓴다.

	//target을 리턴할 것이기 때문에 Actor return이 되는 UEnvQueryItemType_Actor를 사용한다.
	UEnvQueryItemType_Actor::SetContextHelper(ContextData, target);

	//위치를 리턴해야 되는 경우 아래처럼 UEnvQueryItemType_VectorBase를 사용하면 된다.
	//UEnvQueryItemType_VectorBase::
}