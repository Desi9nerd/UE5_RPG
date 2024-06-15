#include "SWeaponLeftArea.h"
#include "Weapons/CWeaponAsset.h"
#include "EngineUtils.h"
#include "Widgets/Input/SSearchBox.h"

void SWeaponTableRow::Construct(const FArguments& InArgs, const TSharedRef<STableViewBase>& InOwnerTable)
{
	Data = InArgs._RowData;//InArgs를 통해서 들어온다.

	SMultiColumnTableRow<FWeaponRowDataPtr>::Construct
	(
		FSuperRowType::FArguments().Style(FEditorStyle::Get(), "TableView.DarkRow"), InOwnerTable
	);
}

TSharedRef<SWidget> SWeaponTableRow::GenerateWidgetForColumn(const FName& InColumnName)
{
	//InColumnName에 아래 Construct 내부의 SHeaderRow::Column("이름, 번호 등")이 들어간다.
	FString str;
	if (InColumnName == "Number")//InColumnName이 숫자면
	{
		str = FString::FromInt(Data->Number);//str에 숫자를 넣어준다.
	}
	else if (InColumnName == "Name")//InColumnName이 이름이면
	{
		str = Data->Name;//str에 이름을 넣어준다.
	}

	return SNew(STextBlock)
		.Text(FText::FromString(str));//출력할 문자str를 생성하여 리턴해준다.
}

/*모양을 디자인 해주는 역할*/
void SWeaponLeftArea::Construct(const FArguments& InArgs)
{
	OnListViewSelectedItem = InArgs._OnSelectedItem;//InArgs안의 OnSelectionItem 사용.

	ChildSlot
		[
			SNew(SVerticalBox)
			+ SVerticalBox::Slot()
		.AutoHeight()
		.Padding(2, 0)
		[
			SAssignNew(SearchBox, SSearchBox)//SearchBox를 가져와 동적할당하여 사용한다.
			.SelectAllTextWhenFocused(true)//검색창 클릭 시 텍스트 입력되있어도 자동으로 입력되게 해주는 기능.
		.OnTextChanged(this, &SWeaponLeftArea::OnTextChanged)
		.OnTextCommitted(this, &SWeaponLeftArea::OnTextCommitted)
		]
	+ SVerticalBox::Slot()
		.FillHeight(1)//1이 100%를 의미한다. 한줄을 다 채우겠다는 의미.
		[
			SAssignNew(ListView, SListView<FWeaponRowDataPtr>)//ListView를 가져와 동적할당하여 사용한다. 자료형은 FWeaponRowDataPtr
			.HeaderRow
			(
				SNew(SHeaderRow)
				+ SHeaderRow::Column("Number")
				.DefaultLabel(FText::FromString(""))
				.ManualWidth(40)//칸의 너비
				+ SHeaderRow::Column("Name")
				.DefaultLabel(FText::FromString("Name"))
			)
		.ListItemsSource(&RowDatas)
		.OnGenerateRow(this, &SWeaponLeftArea::OnGenerateRow)//한줄한줄 어떻게 표현할지 모양을 정해달라는 의미.
		.OnSelectionChanged(this, &SWeaponLeftArea::OnSelectionChanged)
		.SelectionMode(ESelectionMode::Single)
		]
	+ SVerticalBox::Slot()
		.AutoHeight()
		.VAlign(VAlign_Center)
		.HAlign(HAlign_Right)
		.Padding(FMargin(8, 2))
		[
			SNew(STextBlock)
			.Text(this, &SWeaponLeftArea::OnGetAssetCount)//const TAttribute<FText>. 여기의 text 내용이 바뀌면 자동으로 갱신해준다. Attribute의 특성 때문이다. Attribute를 함수로 줄 수 있다. 여기서는 &SWeaponLeftArea::OnGetAssetCount.
		]
		];

	ReadDataAssetList();
}

void SWeaponLeftArea::SelectDataPtr(UCWeaponAsset* InAsset)
{
	if (false == HasRowDataPtr()) return;

	for (FWeaponRowDataPtr ptr : RowDatas)
	{
		if (ptr->Asset == InAsset)
		{
			ListView->SetSelection(ptr);

			return;
		}
	}
}

FWeaponRowDataPtr SWeaponLeftArea::GetRowDataPtrByName(FString InAssetName)
{
	for (FWeaponRowDataPtr ptr : RowDatas)//RowDatas를 모두 탐색했을때
	{
		if (ptr->Name == InAssetName)//ptr의 이름이 받은 InAssetName이 같다면
			return ptr;//그 데이터인 ptr를 리턴.
	}

	return nullptr;
}

FString SWeaponLeftArea::SelectedRowDataPtrName()
{
	TArray<FWeaponRowDataPtr> ptrs = ListView->GetSelectedItems();

	if (ptrs.Num() > 0)//선택된게 있을때
	{
		return ptrs[0]->Asset->GetName();//선택된 것의 이름을 리턴
	}

	return "";//빈 문자열 리턴
}

TSharedRef<ITableRow> SWeaponLeftArea::OnGenerateRow(FWeaponRowDataPtr InRow, const TSharedRef<STableViewBase>& InTable)
{
	/*실제모양을 만들어 리턴*/
	return SNew(SWeaponTableRow, InTable)
		.RowData(InRow);
}

void SWeaponLeftArea::ReadDataAssetList()
{
	RowDatas.Empty();//데이터를 불러오기 전에 비워주고 시작.

	//"/Game"은 콘텐츠 폴더. 모든 에셋들을 찾아 objects에 리턴하겠다.
	///ATL_Class는 클래스 타입, ATL_Regular는 나머지 타입(에셋을 불러와야 하므로 이것 사용).
	TArray<UObject*> objects;
	EngineUtils::FindOrLoadAssetsByPath("/Game/Weapons/", objects, EngineUtils::ATL_Regular);

	int32 index = 0;
	for (UObject* obj : objects)
	{
		UCWeaponAsset* asset = Cast<UCWeaponAsset>(obj);
		if (asset == nullptr) continue;

		FString name = asset->GetName();
		if (SearchText.IsEmpty() == false)//검색하는 문자열(=SearchText)이 비워져있지 않다면(=있다면)
		{
			//에셋에 SearchText 문자열이 포함되는지 확인. 포함되지 않는다면 보여줄게 아니기 때문에 continue; 
			if (name.Contains(SearchText.ToString()) == false)
				continue;
		}

		RowDatas.Add(FWeaponRowData::Make(++index, name, asset));//데이터를 하나씩 추가해준다.
	}

	RowDatas.Sort([](const FWeaponRowDataPtr& A, const FWeaponRowDataPtr& B)
		{
			return A->Number < B->Number;//오름차순 정렬
		});

	ListView->RequestListRefresh();//다른곳에서 사용하기 때문에 재갱신를 해준다.
}

FText SWeaponLeftArea::OnGetAssetCount() const
{
	FString str = FString::Printf(L"%d Asset", RowDatas.Num());//에셋

	return FText::FromString(str);
}

void SWeaponLeftArea::OnTextChanged(const FText& InText)
{
	//기존 문자열과 현재 입력된 문자열이 같다면 리턴으로 끝내버린다.
	if (SearchText.CompareToCaseIgnored(InText) == 0) return;

	SearchText = InText;
	ReadDataAssetList();//재검색하도록 ReadDataAssetList()를 콜 한다.
}

void SWeaponLeftArea::OnTextCommitted(const FText& InText, ETextCommit::Type InType)
{
	OnTextChanged(InText);
}

void SWeaponLeftArea::OnSelectionChanged(FWeaponRowDataPtr InDataPtr, ESelectInfo::Type InType)
{
	//InDataPtr로 선택된 데이터가 들어온다.
	//주의사항: 빈 구역을 클릭해도 InDataPtr의 데이터가 들어온다. 빈 구역 클릭 시 null로 값이 들어온다.
	if (false == InDataPtr.IsValid()) return;

	OnListViewSelectedItem.ExecuteIfBound(InDataPtr);
}