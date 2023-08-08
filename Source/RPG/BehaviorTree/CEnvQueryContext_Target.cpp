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
	
	//Query�� �����ϴ� Actor�� ACEnemy_AI ĳ����
	ACEnemy_AI* ai = Cast<ACEnemy_AI>(QueryInstance.Owner.Get());//QueryInstance.Owner.Get()�� UObject �����̴�. Querier�� Object, Actor, Location ���� �� �� ������ ������̵� �� ������ �ֵ��� �ֻ������� UObject�� �ޱ� ����QueryInstance.Owner.Get()���� �޾Ҵ�.
	ACAIController* controller = ai->GetController<ACAIController>();//Blackboard�� �������� ���� ���� controller�� ĳ�����Ѵ�.
	UBlackboardComponent* blackboard = controller->GetBlackboardComponent();//controller ���� Blackboard�� �����´�.
	AActor* target = Cast<AActor>(blackboard->GetValueAsObject("Target"));//Blackboard�� Target�� �����پ���.

	//target�� ������ ���̱� ������ Actor return�� �Ǵ� UEnvQueryItemType_Actor�� ����Ѵ�.
	UEnvQueryItemType_Actor::SetContextHelper(ContextData, target);

	//��ġ�� �����ؾ� �Ǵ� ��� �Ʒ�ó�� UEnvQueryItemType_VectorBase�� ����ϸ� �ȴ�.
	//UEnvQueryItemType_VectorBase::
}