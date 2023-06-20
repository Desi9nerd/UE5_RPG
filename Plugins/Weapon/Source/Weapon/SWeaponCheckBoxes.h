#pragma once

#include "CoreMinimal.h"

class WEAPON_API SWeaponCheckBoxes
	: public TSharedFromThis<SWeaponCheckBoxes>
{
public:
	//IPropertyHandle는 하나의 Property에 대한 식별자
	void AddProperties(TSharedPtr<IPropertyHandle> InHandle);

	TSharedRef<SWidget> Draw(bool bBackground = false);

private:
	void OnCheckStateChanged(ECheckBoxState InState, int32 InIndex);//CheckBox의 true, false를 바꾸어주는 함수

private:
	//내부 구조체
	struct FInternalData
	{
		bool bChecked;//체크 되었는가
		FString Name;//이름
		TSharedPtr<IPropertyHandle> Handle;//식별자

		FInternalData(TSharedPtr<IPropertyHandle> InHandle)
		{
			bChecked = false;//기본값은 false로 설정.
			Handle = InHandle;

			Name = Handle->GetPropertyDisplayName().ToString();//핸들 내의 DisplayName을 출력이름으로 설정.
		}
	};
	TArray<FInternalData> InternalDatas;//구조체 Data 전부를 포괄한 배열변수
};