#include "WeaponAssetEditor.h"
#include "SWeaponLeftArea.h"
#include "SWeaponDetailsView.h"
#include "SWeaponEquipmentData.h"
#include "SWeaponDoActionData.h"
#include "SWeaponHitData.h"
#include "Weapons/CWeaponAsset.h"

//설정한 이름들
const FName FWeaponAssetEditor::EditorName = "WeaponAssetEditor";
const FName FWeaponAssetEditor::LeftAreaTabId = "LeftArea";
const FName FWeaponAssetEditor::DetailTabId = "Details";

TSharedPtr<FWeaponAssetEditor> FWeaponAssetEditor::Instance = nullptr;//헤더에서 선언한 static 외부 초기화.

void FWeaponAssetEditor::OpenWindow(FString InAssetName)
{
	//아래의 코드 대신에 Shutdown()을 사용하면 안 된다! Shutdown()은 다른 곳에서도 콜이 되기 때문에 아래의 코드 대신에 사용하면 문제가 된다.
	//창이 한번이라도 열리면 인스턴스가 할당된다.
	if (Instance.IsValid()) //창이 만들어졌다면 
	{
		if (Instance->LeftArea.IsValid())//LeftArea가 존재한다면
		{
			FWeaponRowDataPtr ptr = nullptr;

			if (InAssetName.Len() > 0)//콘텐츠 브라우저를 더블클릭 했다면
				ptr = Instance->LeftArea->GetRowDataPtrByName(InAssetName);//InAssetName으로 ptr

			if (ptr.IsValid() == false)//더블클릭 안 했다면 또는 InAssetName을 못 찾았다면
				ptr = Instance->LeftArea->GetFirstDataPtr();//첫번째꺼

			Instance->LeftArea->SelectDataPtr(ptr->Asset);

			return;
		}

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
	LeftArea = SNew(SWeaponLeftArea)//SWeaponLeftArea에서 받은 자료형을 생성하여 넣어준다.
		.OnSelectedItem(this, &FWeaponAssetEditor::OnListViewSelectedItem);//LeftArea에서 선택한 데이터

	FPropertyEditorModule& prop = FModuleManager::LoadModuleChecked<FPropertyEditorModule>("PropertyEditor");

	//DetailsView
	{
		FDetailsViewArgs args(false, false, true, FDetailsViewArgs::HideNameArea);//기본값 설정. ActorsUserNameArea, ObjectsUserNameArea, HideNameArea
		args.ViewIdentifier = "WeaponAssetEditorDetailsView";//식별자 설정. 게임 Editor쪽에서 DetailView 접근시 이 식별자로 찾을 수 있다.
		DetailsView = prop.CreateDetailView(args);//Detail 창 띄우기.

		FOnGetDetailCustomizationInstance detailView;
		detailView.BindStatic(&SWeaponDetailsView::MakeInstance);//Static은 객체가 필요없다. 그래서 함수 주소로 바로 연결한다.
		DetailsView->SetGenericLayoutDetailsDelegate(detailView);//Delegate를 연결해준다.
	}

	//EquipmentData
	{
		FOnGetPropertyTypeCustomizationInstance instance;
		instance.BindStatic(&SWeaponEquipmentData::MakeInstance);
		prop.RegisterCustomPropertyTypeLayout("EquipmentData", instance);//instance를 delegate 등록
	}

	//DoActionData
	{
		FOnGetPropertyTypeCustomizationInstance instance;
		instance.BindStatic(&SWeaponDoActionData::MakeInstance);
		prop.RegisterCustomPropertyTypeLayout("DoActionData", instance);//instance를 delegate 등록
	}

	//HitData
	{
		FOnGetPropertyTypeCustomizationInstance instance;
		instance.BindStatic(&SWeaponHitData::MakeInstance);
		prop.RegisterCustomPropertyTypeLayout("HitData", instance);//instance를 delegate 등록
	}

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
					->AddTab(LeftAreaTabId, ETabState::OpenedTab)//ListViewTabId
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

	UCWeaponAsset* asset = nullptr;
	if (InAssetName.Len() > 0)//받은 InAssetName의 문자열이 0보다 크다면(=에셋이 있다는 의미)
	{
		FWeaponRowDataPtr ptr = LeftArea->GetRowDataPtrByName(InAssetName);

		if (LeftArea->SelectedRowDataPtrName() == InAssetName)//LeftArea의 선택된 이름과 InAssetName이 같다면
			return;

		if (ptr.IsValid())
			asset = ptr->Asset;//ptr의 Asset을 넣어준다.
	}
	if (asset == nullptr)
		asset = LeftArea->GetFirstDataPtr()->Asset;//LeftArea의 첫번째 데이터 선택

	FAssetEditorToolkit::InitAssetEditor(EToolkitMode::Standalone, TSharedPtr<IToolkitHost>(), EditorName, layout, true, true, asset);

	//DetailsView->SetObject(asset);//어느 DetailView든 asset객체가 세팅된다. 창은 여러개지만 실제 관리는 내부적으로 하나로 관리한다. 그래서 창이 종료될 때 DetailsView가 해제 안 되어 있으면 터진다.
	LeftArea->SelectDataPtr(asset);
}

bool FWeaponAssetEditor::OnRequestClose()
{
	if (!!DetailsView)
	{
		//AssetEditorSubsystem안에(=DetailView 안에)
		//GetEditingObject()가 등록되어 있었다면 해제하고 Editor에 알린다.
		if (!!GEditor && !!GEditor->GetEditorSubsystem<UAssetEditorSubsystem>())
			GEditor->GetEditorSubsystem<UAssetEditorSubsystem>()->NotifyAssetClosed(GetEditingObject(), this);

		//해당 모듈이 읽힌적이 있다면 해제.
		if (FModuleManager::Get().IsModuleLoaded("PropertyEditor"))
		{
			FPropertyEditorModule& prop = FModuleManager::LoadModuleChecked<FPropertyEditorModule>("PropertyEditor");//해당모듈을 가져온다.
			prop.UnregisterCustomClassLayout("EquipmentData");//등록 해제
			prop.UnregisterCustomClassLayout("DoActionData");//등록 해제
			prop.UnregisterCustomClassLayout("HitData");//등록 해제
		}
	}

	if (LeftArea.IsValid())
		LeftArea.Reset();

	if (DetailsView.IsValid())
		DetailsView.Reset();

	return true;
	//false 리턴인 경우 창이 닫힐 수 없다는 것을 의미한다.
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
	//	.OnClicked(this, &FWeaponAssetEditor::OnClicked)//OnClicked 함수 연결
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
	if (InDataPtr == nullptr)//LeftArea에서 선택한게 없다면(또는 빈 공간을 선택했다면)
		return;

	if (!!GetEditingObject())//편집하는 객체가 있다면
		RemoveEditingObject(GetEditingObject());//현재 창에서 편집중인 애들 제거한다.

	AddEditingObject(InDataPtr->Asset);//현재 창에 편집해줄 객체를 등록해준다.
	DetailsView->SetObject(InDataPtr->Asset);//창 안의 DetailsView도 변경해준다.
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