#include "HUD/CHealthBarComponent.h"
#include "HUD/CHealthBar.h"
#include "Components/ProgressBar.h"

void UCHealthBarComponent::SetHealthPercent(float Percent)
{
	if (HealthBarWidget == nullptr)
	{	//nullptr üũ���� �׻� ĳ�����ϴ� ���: üũ�ϴ� Ŭ������ ����� ������ �θ��� Ŭ�����鿡���� ����� ã�´�. �׷��� ����� ���� ���. ������ ���� nullptr üũ�� �ϸ� ���ʿ��� ������ ���� �� �ִ�.
		//��, �ش� ��쿡 ����(HealthBarWidget)�� nullptr�� �����ؾ� �Ѵ�. garbage data�� �����ϸ� ������ �� �� �ִ�.
		HealthBarWidget = Cast<UCHealthBar>(GetUserWidgetObject());
	}

	if (HealthBarWidget && HealthBarWidget->HealthBar)
	{
		HealthBarWidget->HealthBar->SetPercent(Percent);
	}
}
