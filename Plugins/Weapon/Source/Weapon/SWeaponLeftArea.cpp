#include "SWeaponLeftArea.h"
#include "Weapons/CWeaponAsset.h"
#include "EngineUtils.h"

void SWeaponTableRow::Construct(const FArguments& InArgs, const TSharedRef<STableViewBase>& InOwnerTable)
{
	Data = InArgs._RowData;//InArgs를 통해서 들어온다.

	SMultiColumnTableRow<FWeaponRowDataPtr>::Construct
	(
		FSuperRowType::FArguments().Style(FEditorStyle::Get(), "TableView.DarkRow"), InOwnerTable
	);
}

TSharedRef<SWidget> SWeaponTableRow::GenerateWidgetForColumn(const FName& InColumnName)
{	//InColumnName에 아래 Construct 내부의 SHeaderRow::Column("이름, 번호 등")이 들어간다.
	FString str;
	if (InColumnName == "Number")//InColumnName이 숫자면
		str = FString::FromInt(Data->Number);//str에 숫자를 넣어준다.
	else if (InColumnName == "Name")//InColumnName이 이름이면
		str = Data->Name;//str에 이름을 넣어준다.

	return SNew(STextBlock)
		.Text(FText::FromString(str));//출력할 문자str를 생성하여 리턴해준다.
}

///////////////////////////////////////////////////////////////////////////

/*모양을 디자인 해주는 역할*/
void SWeaponLeftArea::Construct(const FArguments& InArgs)
{
	ChildSlot
		[
			SNew(SVerticalBox)
			+ SVerticalBox::Slot()
		.FillHeight(1)//1이 100%를 의미한다. 한줄을 다 채우겠다는 의미.
		[
			SAssignNew(ListView, SListView<FWeaponRowDataPtr>)//자료형은 FWeaponRowDataPtr
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

	//테스트용 출력
	//RowDatas.Add(FWeaponRowData::Make(1, "aaa", nullptr));
	//RowDatas.Add(FWeaponRowData::Make(2, "bbb", nullptr));
	//RowDatas.Add(FWeaponRowData::Make(3, "ccc", nullptr));

	ReadDataAssetList();
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

		RowDatas.Add(FWeaponRowData::Make(++index, name, asset));//데이터를 하나씩 넣어준다.
	}

	RowDatas.Sort([](const FWeaponRowDataPtr& A, const FWeaponRowDataPtr& B)
		{
			return A->Number < B->Number;//오름차순 정렬
		});

	ListView->RequestListRefresh();//다른곳에서 사용하기 때문에 재갱신를 해준다.
}

FText SWeaponLeftArea::OnGetAssetCount() const
{
	FString str = FString::Printf(L"%d 에셋", RowDatas.Num());

	return FText::FromString(str);
}