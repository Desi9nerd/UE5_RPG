#include "SWeaponLeftArea.h"
#include "Weapons/CWeaponAsset.h"
#include "EngineUtils.h"
#include "Widgets/Input/SSearchBox.h"

void SWeaponTableRow::Construct(const FArguments& InArgs, const TSharedRef<STableViewBase>& InOwnerTable)
{
	Data = InArgs._RowData;//InArgs�� ���ؼ� ���´�.

	SMultiColumnTableRow<FWeaponRowDataPtr>::Construct
	(
		FSuperRowType::FArguments().Style(FEditorStyle::Get(), "TableView.DarkRow"), InOwnerTable
	);
}

TSharedRef<SWidget> SWeaponTableRow::GenerateWidgetForColumn(const FName& InColumnName)
{
	//InColumnName�� �Ʒ� Construct ������ SHeaderRow::Column("�̸�, ��ȣ ��")�� ����.
	FString str;
	if (InColumnName == "Number")//InColumnName�� ���ڸ�
	{
		str = FString::FromInt(Data->Number);//str�� ���ڸ� �־��ش�.
	}
	else if (InColumnName == "Name")//InColumnName�� �̸��̸�
	{
		str = Data->Name;//str�� �̸��� �־��ش�.
	}

	return SNew(STextBlock)
		.Text(FText::FromString(str));//����� ����str�� �����Ͽ� �������ش�.
}

/*����� ������ ���ִ� ����*/
void SWeaponLeftArea::Construct(const FArguments& InArgs)
{
	OnListViewSelectedItem = InArgs._OnSelectedItem;//InArgs���� OnSelectionItem ���.

	ChildSlot
		[
			SNew(SVerticalBox)
			+ SVerticalBox::Slot()
		.AutoHeight()
		.Padding(2, 0)
		[
			SAssignNew(SearchBox, SSearchBox)//SearchBox�� ������ �����Ҵ��Ͽ� ����Ѵ�.
			.SelectAllTextWhenFocused(true)//�˻�â Ŭ�� �� �ؽ�Ʈ �Էµ��־ �ڵ����� �Էµǰ� ���ִ� ���.
		.OnTextChanged(this, &SWeaponLeftArea::OnTextChanged)
		.OnTextCommitted(this, &SWeaponLeftArea::OnTextCommitted)
		]
	+ SVerticalBox::Slot()
		.FillHeight(1)//1�� 100%�� �ǹ��Ѵ�. ������ �� ä��ڴٴ� �ǹ�.
		[
			SAssignNew(ListView, SListView<FWeaponRowDataPtr>)//ListView�� ������ �����Ҵ��Ͽ� ����Ѵ�. �ڷ����� FWeaponRowDataPtr
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
			.Text(this, &SWeaponLeftArea::OnGetAssetCount)//const TAttribute<FText>. ������ text ������ �ٲ�� �ڵ����� �������ش�. Attribute�� Ư�� �����̴�. Attribute�� �Լ��� �� �� �ִ�. ���⼭�� &SWeaponLeftArea::OnGetAssetCount.
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
	for (FWeaponRowDataPtr ptr : RowDatas)//RowDatas�� ��� Ž��������
	{
		if (ptr->Name == InAssetName)//ptr�� �̸��� ���� InAssetName�� ���ٸ�
			return ptr;//�� �������� ptr�� ����.
	}

	return nullptr;
}

FString SWeaponLeftArea::SelectedRowDataPtrName()
{
	TArray<FWeaponRowDataPtr> ptrs = ListView->GetSelectedItems();

	if (ptrs.Num() > 0)//���õȰ� ������
	{
		return ptrs[0]->Asset->GetName();//���õ� ���� �̸��� ����
	}

	return "";//�� ���ڿ� ����
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
		if (SearchText.IsEmpty() == false)//�˻��ϴ� ���ڿ�(=SearchText)�� ��������� �ʴٸ�(=�ִٸ�)
		{
			//���¿� SearchText ���ڿ��� ���ԵǴ��� Ȯ��. ���Ե��� �ʴ´ٸ� �����ٰ� �ƴϱ� ������ continue; 
			if (name.Contains(SearchText.ToString()) == false)
				continue;
		}

		RowDatas.Add(FWeaponRowData::Make(++index, name, asset));//�����͸� �ϳ��� �߰����ش�.
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

void SWeaponLeftArea::OnTextChanged(const FText& InText)
{
	//���� ���ڿ��� ���� �Էµ� ���ڿ��� ���ٸ� �������� ����������.
	if (SearchText.CompareToCaseIgnored(InText) == 0) return;

	SearchText = InText;
	ReadDataAssetList();//��˻��ϵ��� ReadDataAssetList()�� �� �Ѵ�.
}

void SWeaponLeftArea::OnTextCommitted(const FText& InText, ETextCommit::Type InType)
{
	OnTextChanged(InText);
}

void SWeaponLeftArea::OnSelectionChanged(FWeaponRowDataPtr InDataPtr, ESelectInfo::Type InType)
{
	//InDataPtr�� ���õ� �����Ͱ� ���´�.
	//���ǻ���: �� ������ Ŭ���ص� InDataPtr�� �����Ͱ� ���´�. �� ���� Ŭ�� �� null�� ���� ���´�.
	if (false == InDataPtr.IsValid()) return;

	OnListViewSelectedItem.ExecuteIfBound(InDataPtr);
}