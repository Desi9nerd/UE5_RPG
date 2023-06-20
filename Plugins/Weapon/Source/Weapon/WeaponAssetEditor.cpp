#include "WeaponAssetEditor.h"
#include "SWeaponLeftArea.h"
#include "SWeaponDetailsView.h"
#include "SWeaponEquipmentData.h"
#include "SWeaponDoActionData.h"
#include "SWeaponHitData.h"
#include "Weapons/CWeaponAsset.h"

//������ �̸���
const FName FWeaponAssetEditor::EditorName = "WeaponAssetEditor";
const FName FWeaponAssetEditor::LeftAreaTabId = "LeftArea";
const FName FWeaponAssetEditor::DetailTabId = "Details";

TSharedPtr<FWeaponAssetEditor> FWeaponAssetEditor::Instance = nullptr;//������� ������ static �ܺ� �ʱ�ȭ.

void FWeaponAssetEditor::OpenWindow(FString InAssetName)
{
	//�Ʒ��� �ڵ� ��ſ� Shutdown()�� ����ϸ� �� �ȴ�! Shutdown()�� �ٸ� �������� ���� �Ǳ� ������ �Ʒ��� �ڵ� ��ſ� ����ϸ� ������ �ȴ�.
	//â�� �ѹ��̶� ������ �ν��Ͻ��� �Ҵ�ȴ�.
	if (Instance.IsValid()) //â�� ��������ٸ� 
	{
		if (Instance->LeftArea.IsValid())//LeftArea�� �����Ѵٸ�
		{
			FWeaponRowDataPtr ptr = nullptr;

			if (InAssetName.Len() > 0)//������ �������� ����Ŭ�� �ߴٸ�
				ptr = Instance->LeftArea->GetRowDataPtrByName(InAssetName);//InAssetName���� ptr

			if (ptr.IsValid() == false)//����Ŭ�� �� �ߴٸ� �Ǵ� InAssetName�� �� ã�Ҵٸ�
				ptr = Instance->LeftArea->GetFirstDataPtr();//ù��°��

			Instance->LeftArea->SelectDataPtr(ptr->Asset);

			return;
		}

		Instance->CloseWindow();//â�� �ݴ´�.

		Instance.Reset();
		Instance = nullptr;
	}

	Instance = MakeShareable(new FWeaponAssetEditor());
	Instance->Open(InAssetName);
}

void FWeaponAssetEditor::Shutdown()
{
	if (Instance.IsValid())
	{
		Instance->CloseWindow();

		Instance.Reset();
		Instance = nullptr;
	}
}

void FWeaponAssetEditor::Open(FString InAssetName)
{
	LeftArea = SNew(SWeaponLeftArea)//SWeaponLeftArea���� ���� �ڷ����� �����Ͽ� �־��ش�.
		.OnSelectedItem(this, &FWeaponAssetEditor::OnListViewSelectedItem);//LeftArea���� ������ ������

	FPropertyEditorModule& prop = FModuleManager::LoadModuleChecked<FPropertyEditorModule>("PropertyEditor");

	//DetailsView
	{
		FDetailsViewArgs args(false, false, true, FDetailsViewArgs::HideNameArea);//�⺻�� ����. ActorsUserNameArea, ObjectsUserNameArea, HideNameArea
		args.ViewIdentifier = "WeaponAssetEditorDetailsView";//�ĺ��� ����. ���� Editor�ʿ��� DetailView ���ٽ� �� �ĺ��ڷ� ã�� �� �ִ�.
		DetailsView = prop.CreateDetailView(args);//Detail â ����.

		FOnGetDetailCustomizationInstance detailView;
		detailView.BindStatic(&SWeaponDetailsView::MakeInstance);//Static�� ��ü�� �ʿ����. �׷��� �Լ� �ּҷ� �ٷ� �����Ѵ�.
		DetailsView->SetGenericLayoutDetailsDelegate(detailView);//Delegate�� �������ش�.
	}

	//EquipmentData
	{
		FOnGetPropertyTypeCustomizationInstance instance;
		instance.BindStatic(&SWeaponEquipmentData::MakeInstance);
		prop.RegisterCustomPropertyTypeLayout("EquipmentData", instance);//instance�� delegate ���
	}

	//DoActionData
	{
		FOnGetPropertyTypeCustomizationInstance instance;
		instance.BindStatic(&SWeaponDoActionData::MakeInstance);
		prop.RegisterCustomPropertyTypeLayout("DoActionData", instance);//instance�� delegate ���
	}

	//HitData
	{
		FOnGetPropertyTypeCustomizationInstance instance;
		instance.BindStatic(&SWeaponHitData::MakeInstance);
		prop.RegisterCustomPropertyTypeLayout("HitData", instance);//instance�� delegate ���
	}

	//Layout ����
	TSharedRef<FTabManager::FLayout> layout = FTabManager::NewLayout("WeaponAssetEditor_Layout")
		->AddArea //��üȭ���� ���� ����
		(
			FTabManager::NewPrimaryArea()->SetOrientation(Orient_Vertical)
			->Split
			(
				FTabManager::NewStack()
				->SetSizeCoefficient(0.1f)//10%�� ����ϰڴ�.
				->AddTab(GetToolbarTabId(), ETabState::OpenedTab)
			)
			->Split
			(
				FTabManager::NewSplitter()->SetOrientation(Orient_Horizontal)
				->Split
				(
					FTabManager::NewStack()
					->SetSizeCoefficient(0.175f)//���� 17.5% ���
					->AddTab(LeftAreaTabId, ETabState::OpenedTab)//ListViewTabId
					->SetHideTabWell(true)
				)
				->Split
				(
					FTabManager::NewStack()
					->SetSizeCoefficient(0.725f)//������ 72.5% ���
					->AddTab(DetailTabId, ETabState::OpenedTab)//DetailTabId
					->SetHideTabWell(true)
				)
			)
		);

	UCWeaponAsset* asset = nullptr;
	if (InAssetName.Len() > 0)//���� InAssetName�� ���ڿ��� 0���� ũ�ٸ�(=������ �ִٴ� �ǹ�)
	{
		FWeaponRowDataPtr ptr = LeftArea->GetRowDataPtrByName(InAssetName);

		if (LeftArea->SelectedRowDataPtrName() == InAssetName)//LeftArea�� ���õ� �̸��� InAssetName�� ���ٸ�
			return;

		if (ptr.IsValid())
			asset = ptr->Asset;//ptr�� Asset�� �־��ش�.
	}
	if (asset == nullptr)
		asset = LeftArea->GetFirstDataPtr()->Asset;//LeftArea�� ù��° ������ ����

	FAssetEditorToolkit::InitAssetEditor(EToolkitMode::Standalone, TSharedPtr<IToolkitHost>(), EditorName, layout, true, true, asset);

	//DetailsView->SetObject(asset);//��� DetailView�� asset��ü�� ���õȴ�. â�� ���������� ���� ������ ���������� �ϳ��� �����Ѵ�. �׷��� â�� ����� �� DetailsView�� ���� �� �Ǿ� ������ ������.
	LeftArea->SelectDataPtr(asset);
}

bool FWeaponAssetEditor::OnRequestClose()
{
	if (!!DetailsView)
	{
		//AssetEditorSubsystem�ȿ�(=DetailView �ȿ�)
		//GetEditingObject()�� ��ϵǾ� �־��ٸ� �����ϰ� Editor�� �˸���.
		if (!!GEditor && !!GEditor->GetEditorSubsystem<UAssetEditorSubsystem>())
			GEditor->GetEditorSubsystem<UAssetEditorSubsystem>()->NotifyAssetClosed(GetEditingObject(), this);

		//�ش� ����� �������� �ִٸ� ����.
		if (FModuleManager::Get().IsModuleLoaded("PropertyEditor"))
		{
			FPropertyEditorModule& prop = FModuleManager::LoadModuleChecked<FPropertyEditorModule>("PropertyEditor");//�ش����� �����´�.
			prop.UnregisterCustomClassLayout("EquipmentData");//��� ����
			prop.UnregisterCustomClassLayout("DoActionData");//��� ����
			prop.UnregisterCustomClassLayout("HitData");//��� ����
		}
	}

	if (LeftArea.IsValid())
		LeftArea.Reset();

	if (DetailsView.IsValid())
		DetailsView.Reset();

	return true;
	//false ������ ��� â�� ���� �� ���ٴ� ���� �ǹ��Ѵ�.
}

void FWeaponAssetEditor::RegisterTabSpawners(const TSharedRef<FTabManager>& InTabManager)
{
	FAssetEditorToolkit::RegisterTabSpawners(InTabManager);

	FOnSpawnTab tab;
	tab.BindSP(this, &FWeaponAssetEditor::Spawn_LeftAreaTab);
	TabManager->RegisterTabSpawner(LeftAreaTabId, tab);

	FOnSpawnTab tab2;
	tab2.BindSP(this, &FWeaponAssetEditor::Spawn_DetailsViewTab);
	TabManager->RegisterTabSpawner(DetailTabId, tab2);
}

TSharedRef<SDockTab> FWeaponAssetEditor::Spawn_LeftAreaTab(const FSpawnTabArgs& InArgs)
{
	//TSharedPtr<SDockTab> tab = SNew(SDockTab)
	//[
	//	SNew(SButton)
	//	.OnClicked(this, &FWeaponAssetEditor::OnClicked)//OnClicked �Լ� ����
	//	[
	//		SNew(STextBlock)
	//		.Text(FText::FromString("Test"))
	//	]
	//];

	//return tab.ToSharedRef();

	return SNew(SDockTab)
		[
			LeftArea.ToSharedRef()
		];
}

TSharedRef<SDockTab> FWeaponAssetEditor::Spawn_DetailsViewTab(const FSpawnTabArgs& InArgs)
{
	return SNew(SDockTab)
		[
			DetailsView.ToSharedRef()
		];
}

void FWeaponAssetEditor::OnListViewSelectedItem(FWeaponRowDataPtr InDataPtr)
{
	if (InDataPtr == nullptr)//LeftArea���� �����Ѱ� ���ٸ�(�Ǵ� �� ������ �����ߴٸ�)
		return;

	if (!!GetEditingObject())//�����ϴ� ��ü�� �ִٸ�
		RemoveEditingObject(GetEditingObject());//���� â���� �������� �ֵ� �����Ѵ�.

	AddEditingObject(InDataPtr->Asset);//���� â�� �������� ��ü�� ������ش�.
	DetailsView->SetObject(InDataPtr->Asset);//â ���� DetailsView�� �������ش�.
}

FName FWeaponAssetEditor::GetToolkitFName() const
{
	return EditorName;//�ܺο��� � ������ �̸��� �� �� �����ش�.
}

FText FWeaponAssetEditor::GetBaseToolkitName() const
{
	return FText::FromName(EditorName);//�ܺο��� � ������ �̸��� �� �� �����ش�.
}

FString FWeaponAssetEditor::GetWorldCentricTabPrefix() const
{
	return EditorName.ToString();////�ܺο��� � �ĺ��ڸ� �� �� �����ش�.
}

FLinearColor FWeaponAssetEditor::GetWorldCentricTabColorScale() const
{
	return FLinearColor(0, 0, 1);//�Ķ������� ����.
}

FReply FWeaponAssetEditor::OnClicked()
{
	GLog->Log("Test");

	return FReply::Handled();//Handled()�� ó���ϰ� ������.
}