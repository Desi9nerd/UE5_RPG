#include "Characters/CAIController.h"
#include "Global.h"
#include "GameFramework/Character.h"
#include "BehaviorTree/BehaviorTree.h"
#include "Characters/CEnemy_AI.h"
#include "Components/CAIBehaviorComponent.h"
#include "Perception/AIPerceptionComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Perception/AISenseConfig_Sight.h"

ACAIController::ACAIController()
{
	//CHelpers::CreateActorComponent<UBlackboardComponent>(this, &Blackboard, "Blackboard");//UE4���. UBlackboardComponent* Blackboard
	Blackboard = CreateDefaultSubobject<UBlackboardComponent>(TEXT("Blackboard"));//UE5 ���. TObjectPtr<UBlackboardComponent> Blackboard

	CHelpers::CreateActorComponent<UAIPerceptionComponent>(this, &Perception, "Perception");

	Sight = CreateDefaultSubobject<UAISenseConfig_Sight>("Sight");//CreateDefaultSubobject�� ������ �����Ҵ�, UObject�� ��Ÿ�� �����Ҵ�.
	Sight->SightRadius = 600;//�þ� �ݰ�
	Sight->LoseSightRadius = 800;//�þ߸� �Ҵ� ����
	Sight->PeripheralVisionAngleDegrees = 45;//�þ߰�
	Sight->SetMaxAge(2);

	Sight->DetectionByAffiliation.bDetectEnemies = true;//������ ����o
	Sight->DetectionByAffiliation.bDetectNeutrals = false;//�߸� ����x
	Sight->DetectionByAffiliation.bDetectFriendlies = false;//�Ʊ� ����x

	Perception->ConfigureSense(*Sight);//���� ��ü ����. �þ߰����� ����.
	Perception->SetDominantSense(*Sight->GetSenseImplementation());//�þ߰����� �ֿ켱 ����Ÿ������ ����.
}

void ACAIController::BeginPlay()
{
	Super::BeginPlay();

	Perception->OnPerceptionUpdated.AddDynamic(this, &ACAIController::OnPerceptionUpdated);
}

void ACAIController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	Enemy = Cast<ACEnemy_AI>(InPawn);//Enemy�� possess�Ǵ� Pawn�� �־��ش�.
	SetGenericTeamId(Enemy->GetTeamID());//TeamID ����

	CheckNull(Enemy->GetBehaviorTree());//Enemy�� BehaviorTree�� ������ �ִ��� üũ

	//UE5 ���. UBlackboardComponent*������ ����� Blackboard.Get()�� ����� �� ����ؾ� �Ѵ�.
	UBlackboardComponent* BlackboardTemp = Blackboard.Get();
	UseBlackboard(Enemy->GetBehaviorTree()->BlackboardAsset, BlackboardTemp);//Blackboard ����� ���� BlackboardData�� BlackboardAsset�� BlackboardComonent�� Blackboard�� �Ҵ��Ѵ�.
	this->Blackboard = BlackboardTemp;


	Behavior = CHelpers::GetComponent<UCAIBehaviorComponent>(Enemy);//Enemy���� BehaviorComponent�� �����´�.
	Behavior->SetBlackboard(Blackboard);

	RunBehaviorTree(Enemy->GetBehaviorTree());//BehaviorTree ����
}

void ACAIController::OnUnPossess()
{
	Super::OnUnPossess();
}

void ACAIController::OnPerceptionUpdated(const TArray<AActor*>& UpdatedActors)
{
	TArray<AActor*> actors;//�����Ǵ� ��ü��
	Perception->GetCurrentlyPerceivedActors(nullptr, actors);//Perception ����

	if (actors.Num() > 0)//�����Ǵ� ��ü�� 0 �̻��̸�(=������ ��ü�� �ִٸ�)
	{
		Blackboard->SetValueAsObject("Target", actors[0]);//������ actor[0]�� ��ü�� �����Ѵ�.

		return;
	}

	Blackboard->SetValueAsObject("Target", nullptr);//������ �� �Ǿ��ٸ� nullptr

}
