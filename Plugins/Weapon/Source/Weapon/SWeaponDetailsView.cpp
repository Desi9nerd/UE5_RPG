#include "SWeaponDetailsView.h"
#include "SWeaponCheckBoxes.h"
#include "SWeaponEquipmentData.h"
#include "SWeaponDoActionData.h"
#include "SWeaponHitData.h"
#include "DetailLayoutBuilder.h"
#include "DetailCategoryBuilder.h"
#include "IDetailPropertyRow.h"
#include "Weapons/CWeaponAsset.h"

#include "Animation/AnimMontage.h"
#include "Particles/ParticleSystem.h"
#include "NiagaraSystem.h"
#include "Sound/SoundWave.h"

bool SWeaponDetailsView::bRefreshByCheckBoxes = false;//static ���� �ʱ�ȭ.

TSharedRef<IDetailCustomization> SWeaponDetailsView::MakeInstance()
{
	//�ڽ��� Ŭ���� Ÿ���� ���� return���ش�.
	return MakeShareable(new SWeaponDetailsView());
}

void SWeaponDetailsView::CustomizeDetails(IDetailLayoutBuilder& DetailBuilder)
{
	UClass* type = UCWeaponAsset::StaticClass();//UClass Ÿ�� �ϳ��� �޾ƿ´�.

	//DetailBuilder.HideCategory("CWeaponAsset");//CWeaponAsset ī�װ��� �����ش�. ����� ��� ī�װ��� �����Ű�� ������ ������� �ʴ´�.

	//Class Settings
	{
		//.EditCategory �ش� Ÿ�Կ� �ش� ī�װ��� ������ �� ī�װ��� return, ������ ���� ���� return
		//ClassSettings�� �����Ƿ� ���� ���� return
		IDetailCategoryBuilder& category = DetailBuilder.EditCategory("ClassSettings", FText::FromString("Class Settings"));
		//CWeaponAsset���� ����ȭ�� �������� ī�װ��� �߰��Ѵ�.
		category.AddProperty("AttachmentClass", type);//CWeaponAsset�� AttachmentClass�� ī�װ��� �߰�.
		category.AddProperty("EquipmentClass", type);//CWeaponAsset�� EquipmentClass�� ī�װ��� �߰�.
		category.AddProperty("DoActionClass", type);//CWeaponAsset�� DoActionClass�� ī�װ��� �߰�.
		category.AddProperty("SubActionClass", type);//CWeaponAsset�� SubActionClass�� ī�װ��� �߰�.
	}

	//EquipmentData
	{
		//.EditCategory �ش� Ÿ�Կ� �ش� ī�װ��� ������ �� ī�װ��� return, ������ ���� ���� return
		IDetailCategoryBuilder& category = DetailBuilder.EditCategory("EquipmentData", FText::FromString("Equipment Data"));
		IDetailPropertyRow& row = category.AddProperty("EquipmentData", type);

		if (bRefreshByCheckBoxes == false)//���ΰ�ħ�� �ƴ� ��
		{
			TSharedPtr<SWeaponCheckBoxes> checkBoxes = SWeaponEquipmentData::CreateCheckBoxes();//ī�װ��� ó���� ������� �� checkBox�� �����.
			checkBoxes->AddProperties(row.GetPropertyHandle());//checkBoxes�� ������ ���� Handle�� �߰�

			FEquipmentData data;//���� �����Ѵ�. FEquipmentData�� ���� data �⺻���� �Ʒ����� ����Ѵ�.

			int32 index = 0;
			checkBoxes->CheckDefaultObject(index++, data.Montage);
			checkBoxes->CheckDefaultValue(index++, data.PlayRate);
			checkBoxes->CheckDefaultValue(index++, data.bCanMove);
			checkBoxes->CheckDefaultValue(index++, data.bUseControlRotation);
		}
	}

	//DoActionData
	{
		//.EditCategory �ش� Ÿ�Կ� �ش� ī�װ��� ������ �� ī�װ��� return, ������ ���� ���� return
		IDetailCategoryBuilder& category = DetailBuilder.EditCategory("DoActionData", FText::FromString("DoAction Data"));
		IDetailPropertyRow& row = category.AddProperty("DoActionDatas", type);//���� �߰� //WeaponAsset�� �ִ� �����͸�� ��ġ��Ų��. DoActionDatas

		if (bRefreshByCheckBoxes == false)
		{
			uint32 count = 0;
			row.GetPropertyHandle()->GetNumChildren(count);//��ü ������ ���Ѵ�.

			SWeaponDoActionData::EmptyCheckBoxes();//������� ����.

			FDoActionData data;//�⺻�� ����� ����
			for (uint32 i = 0; i < count; i++)//�ڽ�Handle�� for�� ������
			{
				TSharedPtr<IPropertyHandle> handle = row.GetPropertyHandle()->GetChildHandle(i);//����� handle

				TSharedPtr<SWeaponCheckBoxes> checkBoxes = SWeaponDoActionData::AddCheckBoxes();//ī�װ��� ó���� ������� �� checkBox�� �����.
				checkBoxes->AddProperties(handle);

				int32 index = 0;
				checkBoxes->CheckDefaultObject(index++, data.Montage);
				checkBoxes->CheckDefaultValue(index++, data.PlayRate);
				checkBoxes->CheckDefaultValue(index++, data.bCanMove);
				checkBoxes->CheckDefaultValue(index++, data.bFixedCamera);
				checkBoxes->CheckDefaultObject(index++, data.Effect);
				checkBoxes->CheckDefaultValue(index++, data.EffectLocation);
				checkBoxes->CheckDefaultValue(index++, data.EffectScale);
			}
		}//if(bRefreshByCheckBoxes)
	}

	//HitData
	{
		//.EditCategory �ش� Ÿ�Կ� �ش� ī�װ��� ������ �� ī�װ��� return, ������ ���� ���� return
		IDetailCategoryBuilder& category = DetailBuilder.EditCategory("HitData", FText::FromString("Hit Data"));
		IDetailPropertyRow& row = category.AddProperty("HitDatas", type);//���� �߰� //WeaponAsset�� �ִ� �����͸�� ��ġ��Ų��. DoActionDatas

		if (bRefreshByCheckBoxes == false)
		{
			uint32 count = 0;
			row.GetPropertyHandle()->GetNumChildren(count);//��ü ������ ���Ѵ�.

			SWeaponHitData::EmptyCheckBoxes();//������� ����.

			FHitData data;//�⺻�� ����� ����
			for (uint32 i = 0; i < count; i++)//�ڽ�Handle�� for�� ������
			{
				TSharedPtr<IPropertyHandle> handle = row.GetPropertyHandle()->GetChildHandle(i);//����� handle

				TSharedPtr<SWeaponCheckBoxes> checkBoxes = SWeaponHitData::AddCheckBoxes();//ī�װ��� ó���� ������� �� checkBox�� �����.
				checkBoxes->AddProperties(handle);

				int32 index = 0;
				checkBoxes->CheckDefaultObject(index++, data.Montage);
				checkBoxes->CheckDefaultValue(index++, data.PlayRate);
				checkBoxes->CheckDefaultValue(index++, data.Power);
				checkBoxes->CheckDefaultValue(index++, data.Launch);
				checkBoxes->CheckDefaultValue(index++, data.StopTime);
				checkBoxes->CheckDefaultObject(index++, data.Sound);
				checkBoxes->CheckDefaultObject(index++, data.Effect);
				checkBoxes->CheckDefaultValue(index++, data.EffectLocation);
				checkBoxes->CheckDefaultValue(index++, data.EffectScale);
			}
		}//if(bRefreshByCheckBoxes)
	}

	//DoActionData_AirCombo
	{
		//.EditCategory �ش� Ÿ�Կ� �ش� ī�װ��� ������ �� ī�װ��� return, ������ ���� ���� return
		IDetailCategoryBuilder& category = DetailBuilder.EditCategory("DoActionData_AirCombo", FText::FromString("DoAction Data AirCombo"));
		IDetailPropertyRow& row = category.AddProperty("DoActionDatas_AirCombo", type);//���� �߰� //WeaponAsset�� �ִ� �����͸�� ��ġ��Ų��. DoActionDatas_AirCombo

		if (bRefreshByCheckBoxes == false)
		{
			uint32 count = 0;
			row.GetPropertyHandle()->GetNumChildren(count);//��ü ������ ���Ѵ�.

			SWeaponDoActionData::EmptyCheckBoxes();//������� ����.

			FDoActionData data;//�⺻�� ����� ����
			for (uint32 i = 0; i < count; i++)//�ڽ�Handle�� for�� ������
			{
				TSharedPtr<IPropertyHandle> handle = row.GetPropertyHandle()->GetChildHandle(i);//����� handle

				TSharedPtr<SWeaponCheckBoxes> checkBoxes = SWeaponDoActionData::AddCheckBoxes();//ī�װ��� ó���� ������� �� checkBox�� �����.
				checkBoxes->AddProperties(handle);

				int32 index = 0;
				checkBoxes->CheckDefaultObject(index++, data.Montage);
				checkBoxes->CheckDefaultValue(index++, data.PlayRate);
				checkBoxes->CheckDefaultValue(index++, data.bCanMove);
				checkBoxes->CheckDefaultValue(index++, data.bFixedCamera);
				checkBoxes->CheckDefaultObject(index++, data.Effect);
				checkBoxes->CheckDefaultValue(index++, data.EffectLocation);
				checkBoxes->CheckDefaultValue(index++, data.EffectScale);
			}
		}//if(bRefreshByCheckBoxes)
	}
	//HitData_AirCombo
	{
		//.EditCategory �ش� Ÿ�Կ� �ش� ī�װ��� ������ �� ī�װ��� return, ������ ���� ���� return
		IDetailCategoryBuilder& category = DetailBuilder.EditCategory("HitData_AirCombo", FText::FromString("Hit Data AirCombo"));
		IDetailPropertyRow& row = category.AddProperty("HitDatas_AirCombo", type);//���� �߰� //WeaponAsset�� �ִ� �����͸�� ��ġ��Ų��. DoActionDatas

		if (bRefreshByCheckBoxes == false)
		{
			uint32 count = 0;
			row.GetPropertyHandle()->GetNumChildren(count);//��ü ������ ���Ѵ�.

			SWeaponHitData::EmptyCheckBoxes();//������� ����.

			FHitData data;//�⺻�� ����� ����
			for (uint32 i = 0; i < count; i++)//�ڽ�Handle�� for�� ������
			{
				TSharedPtr<IPropertyHandle> handle = row.GetPropertyHandle()->GetChildHandle(i);//����� handle

				TSharedPtr<SWeaponCheckBoxes> checkBoxes = SWeaponHitData::AddCheckBoxes();//ī�װ��� ó���� ������� �� checkBox�� �����.
				checkBoxes->AddProperties(handle);

				int32 index = 0;
				checkBoxes->CheckDefaultObject(index++, data.Montage);
				checkBoxes->CheckDefaultValue(index++, data.PlayRate);
				checkBoxes->CheckDefaultValue(index++, data.Power);
				checkBoxes->CheckDefaultValue(index++, data.Launch);
				checkBoxes->CheckDefaultValue(index++, data.StopTime);
				checkBoxes->CheckDefaultObject(index++, data.Sound);
				checkBoxes->CheckDefaultObject(index++, data.Effect);
				checkBoxes->CheckDefaultValue(index++, data.EffectLocation);
				checkBoxes->CheckDefaultValue(index++, data.EffectScale);
			}
		}//if(bRefreshByCheckBoxes)
	}
}