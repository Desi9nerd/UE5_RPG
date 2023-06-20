#include "WeaponAssetEditor.h"
#include "Weapons/CWeaponAsset.h"

//������ �̸���
const FName FWeaponAssetEditor::EditorName = "WeaponAssetEditor";
const FName FWeaponAssetEditor::ListViewTabId = "ListView";
const FName FWeaponAssetEditor::DetailTabId = "Details";

TSharedPtr<FWeaponAssetEditor> FWeaponAssetEditor::Instance = nullptr;//������� ������ static �ܺ� �ʱ�ȭ.

void FWeaponAssetEditor::OpenWindow(FString InAssetName)
{
	//�Ʒ��� �ڵ� ��ſ� Shutdown()�� ����ϸ� �� �ȴ�! Shutdown()�� �ٸ� �������� ���� �Ǳ� ������ �Ʒ��� �ڵ� ��ſ� ����ϸ� ������ �ȴ�.
	if (Instance.IsValid()) //â�� ��������ٸ�
	{
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
					->AddTab(ListViewTabId, ETabState::OpenedTab)//ListViewTabId
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

	UCWeaponAsset* asset = NewObject<UCWeaponAsset>();//���� ���� �����̹Ƿ� �ϴ� �� ���·� ������ش�.
	FAssetEditorToolkit::InitAssetEditor(EToolkitMode::Standalone, TSharedPtr<IToolkitHost>(), EditorName, layout, true, true, asset);

}

void FWeaponAssetEditor::RegisterTabSpawners(const TSharedRef<FTabManager>& InTabManager)
{
	FAssetEditorToolkit::RegisterTabSpawners(InTabManager);

	FOnSpawnTab tab;
	tab.BindSP(this, &FWeaponAssetEditor::Spawn_ListViewTab);
	TabManager->RegisterTabSpawner(ListViewTabId, tab);
}

TSharedRef<SDockTab> FWeaponAssetEditor::Spawn_ListViewTab(const FSpawnTabArgs& InArgs)
{
	TSharedPtr<SDockTab> tab = SNew(SDockTab)
		[
			SNew(SButton)
			.OnClicked(this, &FWeaponAssetEditor::OnClicked)//OnClicked �Լ� ����
		[
			SNew(STextBlock)
			.Text(FText::FromString("Test"))
		]
		];

	return tab.ToSharedRef();

	//return SNew(SDockTab);
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