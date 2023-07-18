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

	if(DataTable == nullptr)//DataTable이 없다면
	{   //DataTable이 없다고 메시지를 띄워준다.
		GLog->Log(ELogVerbosity::Error, "DataTable is not selected");

		return;
	}

	OwnerCharacter = Cast<ACharacter>(GetOwner());

	TArray<FMontageData*> datas;
	DataTable->GetAllRows<FMontageData>("", datas);//전체 데이터를 가져온다.

	//가져온 데이터를 배열에 넣어준다.
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
//	//Static이 붙는 애들은 Reflection(자료형의 타입을 변수로 다룰 수 있게 해준다).
//	str.Append(StaticEnum<EStateType>()->GetValueAsString(data->Type));//Enum을 문자열로 바꾸어서 자료형의 이름을 가져온다.
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

	OwnerCharacter->PlayAnimMontage(data->Montage, data->PlayRate);//data의 몽타주를 PlayRate속도로 OwnerCharacter에 적용하여 재생.
}
