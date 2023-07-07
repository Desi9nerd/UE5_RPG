#include "HUD/CHealthBarComponent.h"
#include "HUD/CHealthBar.h"
#include "Components/ProgressBar.h"

void UCHealthBarComponent::SetHealthPercent(float Percent)
{
	if (HealthBarWidget == nullptr)
	{	//nullptr 체크없이 항상 캐스팅하는 경우: 체크하는 클래스에 대상이 없으면 부모의 클래스들에서도 대상을 찾는다. 그래서 비용이 많이 든다. 다음과 같이 nullptr 체크를 하면 불필요한 연산을 줄일 수 있다.
		//단, 해당 경우에 변수(HealthBarWidget)는 nullptr로 시작해야 한다. garbage data로 시작하면 문제가 될 수 있다.
		HealthBarWidget = Cast<UCHealthBar>(GetUserWidgetObject());
	}

	if (HealthBarWidget && HealthBarWidget->HealthBar)
	{
		HealthBarWidget->HealthBar->SetPercent(Percent);
	}
}
