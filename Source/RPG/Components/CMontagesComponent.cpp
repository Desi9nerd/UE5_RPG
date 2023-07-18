#include "CMontagesComponent.h"
#include "Global.h"
#include "GameFramework/Character.h"

//#define LOG_UCMontagesComponent 1

UCMontagesComponent::UCMontagesComponent()
{
}

void UCMontagesComponent::BeginPlay()
{
	Super::BeginPlay();

	if(DataTable == nullptr)//DataTable�� ���ٸ�
	{   //DataTable�� ���ٰ� �޽����� ����ش�.
		GLog->Log(ELogVerbosity::Error, "DataTable is not selected");

		return;
	}

	OwnerCharacter = Cast<ACharacter>(GetOwner());

	TArray<FMontageData*> datas;
	DataTable->GetAllRows<FMontageData>("", datas);//��ü �����͸� �����´�.

	//������ �����͸� �迭�� �־��ش�.
	for (int32 i = 0; i< (int32)EStateType::Max; i++)
	{
		for(FMontageData* data : datas)
		{
			if((EStateType)i == data->Type)
			{
				Datas[i] = data;

				continue;
			}
		}//for(data)
	}//for(i)
}

//
//#if LOG_UCMontagesComponent
//for (FMontagesData* data : datas)
//{
//	if (data == nullptr)
//		continue;
//
//	FString str;
//	//Static�� �ٴ� �ֵ��� Reflection(�ڷ����� Ÿ���� ������ �ٷ� �� �ְ� ���ش�).
//	str.Append(StaticEnum<EStateType>()->GetValueAsString(data->Type));//Enum�� ���ڿ��� �ٲپ �ڷ����� �̸��� �����´�.
//	str.Append(" / ");
//	str.Append(data->Montage->GetPathName());
//
//	CLog::Log(str);
//}
//#endif

void UCMontagesComponent::PlayBackStepMode()
{
	PlayAnimMontage(EStateType::BackStep);
}

void UCMontagesComponent::PlayDeadMode()
{
	PlayAnimMontage(EStateType::Dead);
}

void UCMontagesComponent::PlayAnimMontage(EStateType InType)
{
	CheckNull(OwnerCharacter);

	FMontageData* data = Datas[(int32)InType];

	if(data == nullptr || data->Montage == nullptr)
	{
		GLog->Log(ELogVerbosity::Error, "None montages data");

		return;
	}

	OwnerCharacter->PlayAnimMontage(data->Montage, data->PlayRate);//data�� ��Ÿ�ָ� PlayRate�ӵ��� OwnerCharacter�� �����Ͽ� ���.
}
