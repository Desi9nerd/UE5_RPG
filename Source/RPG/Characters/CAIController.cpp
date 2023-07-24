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
	//CHelpers::CreateActorComponent<UBlackboardComponent>(this, &Blackboard, "Blackboard");//UE4방식. UBlackboardComponent* Blackboard
	Blackboard = CreateDefaultSubobject<UBlackboardComponent>(TEXT("Blackboard"));//UE5 방식. TObjectPtr<UBlackboardComponent> Blackboard

	CHelpers::CreateActorComponent<UAIPerceptionComponent>(this, &Perception, "Perception");

	Sight = CreateDefaultSubobject<UAISenseConfig_Sight>("Sight");//CreateDefaultSubobject는 생성자 동적할당, UObject는 런타임 동적할당.
	Sight->SightRadius = 600;//시야 반경
	Sight->LoseSightRadius = 800;//시야를 잃는 범위
	Sight->PeripheralVisionAngleDegrees = 45;//시야각
	Sight->SetMaxAge(2);

	Sight->DetectionByAffiliation.bDetectEnemies = true;//적들을 감지o
	Sight->DetectionByAffiliation.bDetectNeutrals = false;//중립 감지x
	Sight->DetectionByAffiliation.bDetectFriendlies = false;//아군 감지x

	Perception->ConfigureSense(*Sight);//감지 객체 지정. 시야감지로 지정.
	Perception->SetDominantSense(*Sight->GetSenseImplementation());//시야감지를 최우선 감지타입으로 설정.
}

void ACAIController::BeginPlay()
{
	Super::BeginPlay();

	Perception->OnPerceptionUpdated.AddDynamic(this, &ACAIController::OnPerceptionUpdated);
}

void ACAIController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	Enemy = Cast<ACEnemy_AI>(InPawn);//Enemy에 possess되는 Pawn을 넣어준다.
	SetGenericTeamId(Enemy->GetTeamID());//TeamID 지정

	CheckNull(Enemy->GetBehaviorTree());//Enemy가 BehaviorTree를 가지고 있는지 체크

	//UE5 방식. UBlackboardComponent*변수를 만들어 Blackboard.Get()을 담아준 후 사용해야 한다.
	UBlackboardComponent* BlackboardTemp = Blackboard.Get();
	UseBlackboard(Enemy->GetBehaviorTree()->BlackboardAsset, BlackboardTemp);//Blackboard 사용을 위해 BlackboardData인 BlackboardAsset과 BlackboardComonent인 Blackboard를 할당한다.
	this->Blackboard = BlackboardTemp;


	Behavior = CHelpers::GetComponent<UCAIBehaviorComponent>(Enemy);//Enemy내의 BehaviorComponent를 가져온다.
	Behavior->SetBlackboard(Blackboard);

	RunBehaviorTree(Enemy->GetBehaviorTree());//BehaviorTree 실행
}

void ACAIController::OnUnPossess()
{
	Super::OnUnPossess();
}

void ACAIController::OnPerceptionUpdated(const TArray<AActor*>& UpdatedActors)
{
	TArray<AActor*> actors;//감지되는 객체들
	Perception->GetCurrentlyPerceivedActors(nullptr, actors);//Perception 감지

	if (actors.Num() > 0)//감지되는 객체가 0 이상이면(=감지된 객체가 있다면)
	{
		Blackboard->SetValueAsObject("Target", actors[0]);//감지된 actor[0]을 객체로 지정한다.

		return;
	}

	Blackboard->SetValueAsObject("Target", nullptr);//감지가 안 되었다면 nullptr

}
