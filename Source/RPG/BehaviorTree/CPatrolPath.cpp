#include "BehaviorTree/CPatrolPath.h"
#include "Global.h"
#include "Components/SplineComponent.h"
#include "Components/TextRenderComponent.h"

ACPatrolPath::ACPatrolPath()
{
	bRunConstructionScriptOnDrag = false;

	CHelpers::CreateComponent<USceneComponent>(this, &Root, "Root");
	CHelpers::CreateComponent<USplineComponent>(this, &Spline, "Spline", Root);
	CHelpers::CreateComponent<UTextRenderComponent>(this, &Text, "Text", Root);

	Spline->SetRelativeLocation(FVector(0, 0, 30));
	Spline->bHiddenInGame = false;

	Text->SetRelativeLocation(FVector(0, 0, 120));
	Text->SetRelativeRotation(FRotator(0, 180, 0));
	Text->HorizontalAlignment = EHorizTextAligment::EHTA_Center;
	Text->TextRenderColor = FColor::Red;
	Text->bHiddenInGame = true;
}

void ACPatrolPath::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);

#if WITH_EDITOR
	Text->Text = FText::FromString(GetActorLabel());//GetActorLabel()�� Editor�� �ִ� �Լ��� Project Package �ÿ��� ������� �Ѵ�.
#endif

	Spline->SetClosedLoop(bLoop);
}

void ACPatrolPath::BeginPlay()
{
	Super::BeginPlay();
}

FVector ACPatrolPath::GetMoveTo()
{
	//���� Spline Point �̵��� ���� Spline�� ���� Point�� World ��ġ�� ����. 
	return Spline->GetLocationAtSplinePoint(Index, ESplineCoordinateSpace::World);
}

void ACPatrolPath::UpdateIndex()
{
	int32 count = Spline->GetNumberOfSplinePoints();//cout������ Spline Points ���ڸ� �־��ش�.

	if (bReverse)//������
	{
		if (Index > 0)
		{
			Index--;

			return;
		}

		if (Spline->IsClosedLoop())
		{
			Index = count - 1;

			return;
		}

		Index = 1;
		bReverse = false;

		return;
	}

	if (Index < count - 1)//������
	{
		Index++;

		return;
	}

	if (Spline->IsClosedLoop())
	{
		Index = 0;

		return;
	}

	Index = count - 2;
	bReverse = true;//���������� �����.
}