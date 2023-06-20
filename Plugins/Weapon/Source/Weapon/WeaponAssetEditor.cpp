#include "WeaponAssetEditor.h"
#include "Weapons/CWeaponAsset.h"

//설정한 이름들
const FName FWeaponAssetEditor::EditorName = "WeaponAssetEditor";
const FName FWeaponAssetEditor::ListViewTabId = "ListView";
const FName FWeaponAssetEditor::DetailTabId = "Details";

TSharedPtr<FWeaponAssetEditor> FWeaponAssetEditor::Instance = nullptr;//헤더에서 선언한 static 외부 초기화.

void FWeaponAssetEditor::OpenWindow(FString InAssetName)
{
	//아래의 코드 대신에 Shutdown()을 사용하면 안 된다! Shutdown()은 다른 곳에서도 콜이 되기 때문에 아래의 코드 대신에 사용하면 문제가 된다.
	if (Instance.IsValid()) //창이 만들어졌다면
	{
		Instance->CloseWindow();//창을 닫는다.

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
	//Layout 설정
	TSharedRef<FTabManager::FLayout> layout = FTabManager::NewLayout("WeaponAssetEditor_Layout")
		->AddArea //전체화면의 메인 영역
		(
			FTabManager::NewPrimaryArea()->SetOrientation(Orient_Vertical)
			->Split
			(
				FTabManager::NewStack()
				->SetSizeCoefficient(0.1f)//10%만 사용하겠다.
				->AddTab(GetToolbarTabId(), ETabState::OpenedTab)
			)
			->Split
			(
				FTabManager::NewSplitter()->SetOrientation(Orient_Horizontal)
				->Split
				(
					FTabManager::NewStack()
					->SetSizeCoefficient(0.175f)//왼쪽 17.5% 사용
					->AddTab(ListViewTabId, ETabState::OpenedTab)//ListViewTabId
					->SetHideTabWell(true)
				)
				->Split
				(
					FTabManager::NewStack()
					->SetSizeCoefficient(0.725f)//오른쪽 72.5% 사용
					->AddTab(DetailTabId, ETabState::OpenedTab)//DetailTabId
					->SetHideTabWell(true)
				)
			)
		);

	UCWeaponAsset* asset = NewObject<UCWeaponAsset>();//현재 없는 상태이므로 일단 빈 상태로 만들어준다.
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
			.OnClicked(this, &FWeaponAssetEditor::OnClicked)//OnClicked 함수 연결
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
	return EditorName;//외부에서 어떤 에디터 이름을 쓸 지 정해준다.
}

FText FWeaponAssetEditor::GetBaseToolkitName() const
{
	return FText::FromName(EditorName);//외부에서 어떤 에디터 이름을 쓸 지 정해준다.
}

FString FWeaponAssetEditor::GetWorldCentricTabPrefix() const
{
	return EditorName.ToString();////외부에서 어떤 식별자를 쓸 지 정해준다.
}

FLinearColor FWeaponAssetEditor::GetWorldCentricTabColorScale() const
{
	return FLinearColor(0, 0, 1);//파란색으로 설정.
}

FReply FWeaponAssetEditor::OnClicked()
{
	GLog->Log("Test");

	return FReply::Handled();//Handled()는 처리하고 끝낸다.
}