#include "BehaviorTree/CBTService_Range.h"
#include "Global.h"
#include "GameFramework/Character.h"
#include "Characters/CAIController.h"
#include "Characters/CEnemy_AI.h"
#include "Components/CStateComponent.h"
#include "Components/CAIBehaviorComponent.h"

UCBTService_Range::UCBTService_Range()
{
	NodeName = "Range";

	Interval = 0.1f;//ȣ�� ����. 0.1�� ���� ȣ��
	RandomDeviation = 0.0f;
}

void UCBTService_Range::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	ACAIController* controller = Cast<ACAIController>(OwnerComp.GetOwner());
	ACEnemy_AI* ai = Cast<ACEnemy_AI>(controller->GetPawn());
	UCStateComponent* state = CHelpers::GetComponent<UCStateComponent>(ai);
	UCAIBehaviorComponent* aiState = CHelpers::GetComponent<UCAIBehaviorComponent>(ai);

	if (bDrawDebug)
	{
		FVector start = ai->GetActorLocation();//ACEnemy_AI�� ��ġ
		start.Z -= 25;

		FVector end = start;
		end.Z += 50;

		DrawDebugCylinder(ai->GetWorld(), start, end, AvoidRange, 10, FColor::Red, true, Interval, 0, 0);//����� ����� �׸���
	}

	if (state->IsHittedMode())
	{
		aiState->SetHittedMode();

		return;
	}

	ACharacter* target = aiState->GetTarget();
	if (target == nullptr)//target�� ���ٸ�(=�þ� ���� ���� ���� ���ٸ�)
	{
		//EAIFocusPriorty: Gameplay(Gameplay���), LastFocusPriority(������ Focus �����ָ� �켱�������� ����), Move(�̵� Focus)
		controller->ClearFocus(EAIFocusPriority::Gameplay);//�ٶ󺸴� Focus�� Gameplay ��忡�� ����
		aiState->SetWaitMode();//Ÿ���� ���ٸ� Wait���� ������ش�.

		return;
	}

	//������ �Ǿ��� ��
	controller->SetFocus(target);//������ �� target������ SetFocus�Ͽ� �ٶ󺸰� �����.

	float distance = ai->GetDistanceTo(target);//���� ���� ��ġ���� target������ �Ÿ��� ���Ѵ�.
	if (distance < AvoidRange)//�Ÿ��� ������ ���ݹ��� ���� �۴ٸ�
	{
		aiState->SetAvoidMode();//Avoid ���� ������ش�.

		return;//ȸ�Ǹ� �ϰ� ����
	}

	aiState->SetActionMode();//Action ���� ������ش�. BT_Range�� Action ������ �ؿ� Equip, Action�� �����Ͽ� ���⸦ �����ϰ� �����ϰ� �����.
}
