#include "SWeaponDetailsView.h"
#include "SWeaponCheckBoxes.h"
#include "SWeaponEquipmentData.h"
#include "SWeaponDoActionData.h"
#include "DetailLayoutBuilder.h"
#include "DetailCategoryBuilder.h"
#include "IDetailPropertyRow.h"
#include "Weapons/CWeaponAsset.h"
#include "Animation/AnimMontage.h"

bool SWeaponDetailsView::bRefreshByCheckBoxes = false;//static 변수 초기화.

TSharedRef<IDetailCustomization> SWeaponDetailsView::MakeInstance()
{
	//자신의 클래스 타입을 만들어서 return해준다.
	return MakeShareable(new SWeaponDetailsView());
}

void SWeaponDetailsView::CustomizeDetails(IDetailLayoutBuilder& DetailBuilder)
{
	UClass* type = UCWeaponAsset::StaticClass();//UClass 타입 하나를 받아온다.

	DetailBuilder.HideCategory("CWeaponAsset");//CWeaponAsset 카테고리를 숨겨준다. 현재 작업의 편의를 위해 숨겨주고 추후에 보여주게 만들 예정이다. 

	//Class Settings
	{
		//.EditCategory 해당 타입에 해당 카테고리가 있으면 그 카테고리를 return, 없으면 새로 만들어서 return
		//ClassSettings는 없으므로 새로 만들어서 return
		IDetailCategoryBuilder& category = DetailBuilder.EditCategory("ClassSettings", FText::FromString("Class Settings"));
		//CWeaponAsset에서 직렬화된 변수들을 카테고리에 추가한다.
		category.AddProperty("AttachmentClass", type);//CWeaponAsset의 AttachmentClass를 카테고리에 추가.
		category.AddProperty("EquipmentClass", type);//CWeaponAsset의 EquipmentClass를 카테고리에 추가.
		category.AddProperty("DoActionClass", type);//CWeaponAsset의 DoActionClass를 카테고리에 추가.
	}

	//EquipmentData
	{
		//.EditCategory 해당 타입에 해당 카테고리가 있으면 그 카테고리를 return, 없으면 새로 만들어서 return
		IDetailCategoryBuilder& category = DetailBuilder.EditCategory("EquipmentData", FText::FromString("Equipment Data"));
		IDetailPropertyRow& row = category.AddProperty("EquipmentData", type);

		if (bRefreshByCheckBoxes == false)//새로고침이 아닐 때
		{
			TSharedPtr<SWeaponCheckBoxes> checkBoxes = SWeaponEquipmentData::CreateCheckBoxes();//카테고리가 처음에 만들어질 때 checkBox를 만든다.
			checkBoxes->AddProperties(row.GetPropertyHandle());//checkBoxes에 실제로 가진 Handle를 추가

			FEquipmentData data;//변수 선언한다. FEquipmentData의 변수 data 기본값을 아래에서 사용한다.

			int32 index = 0;
			checkBoxes->CheckDefaultObject(index++, data.Montage);
			checkBoxes->CheckDefaultValue(index++, data.PlayRate);
			checkBoxes->CheckDefaultValue(index++, data.bCanMove);
			checkBoxes->CheckDefaultValue(index++, data.bUseControlRotation);
		}
	}

	//DoActionData
	{
		//.EditCategory 해당 타입에 해당 카테고리가 있으면 그 카테고리를 return, 없으면 새로 만들어서 return
		IDetailCategoryBuilder& category = DetailBuilder.EditCategory("DoActionData", FText::FromString("DoAction Data"));
		IDetailPropertyRow& row = category.AddProperty("DoActionDatas", type);//변수 추가 //WeaponAsset에 있는 데이터명과 일치시킨다. DoActionDatas

		if (bRefreshByCheckBoxes == false)
		{
			uint32 count = 0;
			row.GetPropertyHandle()->GetNumChildren(count);

			SWeaponDoActionData::EmptyCheckBoxes();//비워놓고 시작.

			FDoActionData data;//기본값 사용할 변수
			for (uint32 i = 0; i < count; i++)
			{
				TSharedPtr<IPropertyHandle> handle = row.GetPropertyHandle()->GetChildHandle(i);//헤더의 handle

				TSharedPtr<SWeaponCheckBoxes> checkBoxes = SWeaponDoActionData::AddCheckBoxes();//카테고리가 처음에 만들어질 때 checkBox를 만든다.
				checkBoxes->AddProperties(handle);
			}
		}//if(bRefreshByCheckBoxes)
	}
}