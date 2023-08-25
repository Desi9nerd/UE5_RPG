#pragma once

#include "CoreMinimal.h"

class WEAPON_API SWeaponCheckBoxes
    : public TSharedFromThis<SWeaponCheckBoxes> // 직접적으로 상속받으면 주소가 일치하게된다.
{
public:
    //IPropertyHandle는 하나의 Property에 대한 식별자
    void AddProperties(TSharedPtr<IPropertyHandle> InHandle);

    //SNew에 최상의 부모가 SWidget이 된다. 레퍼런스를 사용하기 때문에 동일하게 쓰기위함
    TSharedRef<SWidget> Draw(bool bBackground = false);//디폴트 파라미터로 기본값을 false로 설정.

    //해당 Properties를 그릴지 말지 결정
    void DrawProperties(TSharedRef<IPropertyHandle> InPropertyHandle, IDetailChildrenBuilder* InChildrenBuilder);

    //객체를 외부에서 받기위해,
    void SetUtilities(TSharedPtr<class IPropertyUtilities> InUtilities);

private:
    //체크가 완료되었는지 
    void OnCheckStateChanged(ECheckBoxState InState, int32 InIndex);

public:
    static bool CanDraw(TSharedPtr<IPropertyHandle> InHandle, int InCount);//사용자가 체크한 InHandle이 들어와서 배열이 개수와 일치하는지 판단. 배열 범위를 벗어나면 그리지 않도록 false를 리턴.

public:
    //InIndex에는 Property 번호가 InValue에는 값.
    void CheckDefaultObject(int32 InIndex, UObject* InValue);
    void CheckDefaultValue(int32 InIndex, float InValue);
    void CheckDefaultValue(int32 InIndex, bool InValue);
    void CheckDefaultValue(int32 InIndex, const FVector& InValue);
    void CheckDefaultClass(int32 InIndex, const TSubclassOf<ACharacter> InClass);

private:
    //내부 구조체 생성, 관리를 위해 사용
    struct FInternalData
    {
        bool bChecked;//체크 되었는가
        FString Name;//이름
        TSharedPtr<IPropertyHandle> Handle;

        FInternalData(TSharedPtr<IPropertyHandle> InHandle)
        {
            bChecked = false;//기본값은 false로 설정.
            Handle = InHandle;

            Name = Handle->GetPropertyDisplayName().ToString();//핸들 내의 DisplayName을 출력이름으로 설정.
        }
    };
    TArray<FInternalData> InternalDatas;//구조체 Data 전부를 포괄한 배열변수

    TSharedPtr<class IPropertyUtilities> Utilities;//멤버 변수
};