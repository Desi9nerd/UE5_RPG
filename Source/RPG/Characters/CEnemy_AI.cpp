#include "Characters/CEnemy_AI.h"
#include "Global.h"
#include "Components/CWeaponComponent.h"
#include "Components/CAIBehaviorComponent.h"
#include "Components/WidgetComponent.h"
#include "Components/CStatusComponent.h"
#include "Widgets/CUserWidget_Label.h"
#include "Kismet/GameplayStatics.h"

ACEnemy_AI::ACEnemy_AI()
{
	PrimaryActorTick.bCanEverTick = true;

	CHelpers::CreateComponent<UWidgetComponent>(this, &LabelWidget, "Label", GetMesh());	
	CHelpers::CreateActorComponent<UCAIBehaviorComponent>(this, &Behavior, "Behavior");

	TSubclassOf<UCUserWidget_Label> labelClass;
	CHelpers::GetClass<UCUserWidget_Label>(&labelClass, "WidgetBlueprint'/Game/Widgets/WB_Label.WB_Label_C'");
	LabelWidget->SetWidgetClass(labelClass);
	LabelWidget->SetRelativeLocation(FVector(0, 0, 220));
	LabelWidget->SetDrawSize(FVector2D(120, 0));
	LabelWidget->SetWidgetSpace(EWidgetSpace::Screen);
}

void ACEnemy_AI::BeginPlay()
{
	Super::BeginPlay();

	LabelWidget->InitWidget();

	UCUserWidget_Label* label = Cast<UCUserWidget_Label>(LabelWidget->GetUserWidgetObject());
	label->UpdateHealth(Status->GetHealth(), Status->GetMaxHealth());
	label->UpdateName(GetName());
	label->UpdateControllerName(GetController()->GetName());

	//TeamID = 2;//TeamID�� 0~255������ ���������ϴ�. 255���� �߸��̴�. ID ������ �Ʊ��̰� ID�� �ٸ��� ���̴�.
}

void ACEnemy_AI::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	UCUserWidget_Label* label = Cast<UCUserWidget_Label>(LabelWidget->GetUserWidgetObject());

	if (!!label)
	{
		label->UpdateHealth(Status->GetHealth(), Status->GetMaxHealth());

		UpdateLabelRenderScale();
	}

	float distanceToPlayer = GetDistanceTo(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
	if (distanceToPlayer < 1000.0f)
		LabelWidget->SetVisibility(true);
	else
		LabelWidget->SetVisibility(false);
}

void ACEnemy_AI::UpdateLabelRenderScale()
{
	UCUserWidget_Label* label = Cast<UCUserWidget_Label>(LabelWidget->GetUserWidgetObject());
	CheckNull(label);


	APlayerCameraManager* cameraManager = UGameplayStatics::GetPlayerCameraManager(GetWorld(), 0);

	FVector cameraLocation = cameraManager->GetCameraLocation();
	FVector targetLocation = GetController()->GetTargetLocation();

	float distance = FVector::Distance(cameraLocation, targetLocation);
	float sizeRate = 1.0f - (distance / LabelViewAmount);

	if (distance > LabelViewAmount)
	{
		label->SetVisibility(ESlateVisibility::Collapsed);

		return;
	}

	label->SetVisibility(ESlateVisibility::Visible);
	label->SetRenderScale(FVector2D(sizeRate, sizeRate));
}

void ACEnemy_AI::Hitted()
{
	Super::Hitted();

	CheckTrue(State->IsDeadMode());//�̹� DeadMode�� Hitted ó���� �ʿ����.

	Behavior->SetHittedMode();//HittedMode�� ������ش�.
}

void ACEnemy_AI::End_Hitted()
{
	Super::End_Hitted();

	Behavior->SetWaitMode();//Hitted�� ������ Behavior�� WaitMode�� ������ش�.
}
