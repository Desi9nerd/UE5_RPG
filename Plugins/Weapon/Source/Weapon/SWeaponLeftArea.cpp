#include "SWeaponLeftArea.h"
#include "Weapons/CWeaponAsset.h"
#include "EngineUtils.h"

void SWeaponTableRow::Construct(const FArguments& InArgs, const TSharedRef<STableViewBase>& InOwnerTable)
{
	Data = InArgs._RowData;//InArgs�� ���ؼ� ���´�.

	SMultiColumnTableRow<FWeaponRowDataPtr>::Construct
	(
		FSuperRowType::FArguments().Style(FEditorStyle::Get(), "TableView.DarkRow"), InOwnerTable
	);
}

TSharedRef<SWidget> SWeaponTableRow::GenerateWidgetForColumn(const FName& InColumnName)
{	//InColumnName�� �Ʒ� Construct ������ SHeaderRow::Column("�̸�, ��ȣ ��")�� ����.
	FString str;
	if (InColumnName == "Number")//InColumnName�� ���ڸ�
		str = FString::FromInt(Data->Number);//str�� ���ڸ� �־��ش�.
	else if (InColumnName == "Name")//InColumnName�� �̸��̸�
		str = Data->Name;//str�� �̸��� �־��ش�.

	return SNew(STextBlock)
		.Text(FText::FromString(str));//����� ����str�� �����Ͽ� �������ش�.
}

///////////////////////////////////////////////////////////////////////////

/*����� ������ ���ִ� ����*/
void SWeaponLeftArea::Construct(const FArguments& InArgs)
{
	ChildSlot
		[
			SNew(SVerticalBox)
			+ SVerticalBox::Slot()
		.FillHeight(1)//1�� 100%�� �ǹ��Ѵ�. ������ �� ä��ڴٴ� �ǹ�.
		[
			SAssignNew(ListView, SListView<FWeaponRowDataPtr>)//�ڷ����� FWeaponRowDataPtr
			.HeaderRow
			(
				SNew(SHeaderRow)
				+ SHeaderRow::Column("Number")
				.DefaultLabel(FText::FromString(""))
				.ManualWidth(40)//ĭ�� �ʺ�
				+ SHeaderRow::Column("Name")
				.DefaultLabel(FText::FromString("Name"))
			)
		.ListItemsSource(&RowDatas)
		.OnGenerateRow(this, &SWeaponLeftArea::OnGenerateRow)//�������� ��� ǥ������ ����� ���ش޶�� �ǹ�.
		]
	+ SVerticalBox::Slot()
		.AutoHeight()
		.VAlign(VAlign_Center)
		.HAlign(HAlign_Right)
		.Padding(FMargin(8, 2))
		[
			SNew(STextBlock)
			.Text(this, &SWeaponLeftArea::OnGetAssetCount)//const TAttribute<FText>. ������ text ������ �ٲ�� �ڵ����� �������ش�. Attribute�� Ư�� �����̴�. Attribute�� �Լ��� �� �� �ִ�. ���⼭�� &SWeaponLeftArea::OnGetAssetCount.
		]
		];

	//�׽�Ʈ�� ���
	//RowDatas.Add(FWeaponRowData::Make(1, "aaa", nullptr));
	//RowDatas.Add(FWeaponRowData::Make(2, "bbb", nullptr));
	//RowDatas.Add(FWeaponRowData::Make(3, "ccc", nullptr));

	ReadDataAssetList();
}

TSharedRef<ITableRow> SWeaponLeftArea::OnGenerateRow(FWeaponRowDataPtr InRow, const TSharedRef<STableViewBase>& InTable)
{
	/*��������� ����� ����*/
	return SNew(SWeaponTableRow, InTable)
		.RowData(InRow);
}

void SWeaponLeftArea::ReadDataAssetList()
{
	RowDatas.Empty();//�����͸� �ҷ����� ���� ����ְ� ����.

	//"/Game"�� ������ ����. ��� ���µ��� ã�� objects�� �����ϰڴ�.
	///ATL_Class�� Ŭ���� Ÿ��, ATL_Regular�� ������ Ÿ��(������ �ҷ��;� �ϹǷ� �̰� ���).
	TArray<UObject*> objects;
	EngineUtils::FindOrLoadAssetsByPath("/Game/Weapons/", objects, EngineUtils::ATL_Regular);

	int32 index = 0;
	for (UObject* obj : objects)
	{
		UCWeaponAsset* asset = Cast<UCWeaponAsset>(obj);
		if (asset == nullptr) continue;

		FString name = asset->GetName();

		RowDatas.Add(FWeaponRowData::Make(++index, name, asset));//�����͸� �ϳ��� �־��ش�.
	}

	RowDatas.Sort([](const FWeaponRowDataPtr& A, const FWeaponRowDataPtr& B)
		{
			return A->Number < B->Number;//�������� ����
		});

	ListView->RequestListRefresh();//�ٸ������� ����ϱ� ������ �簻�Ÿ� ���ش�.
}

FText SWeaponLeftArea::OnGetAssetCount() const
{
	FString str = FString::Printf(L"%d Asset", RowDatas.Num());//����

	return FText::FromString(str);
}