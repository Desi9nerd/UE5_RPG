#pragma once

#include "CoreMinimal.h"
#include "Widgets/SCompoundWidget.h"
#include "Widgets/Views/STableRow.h"

struct FWeaponRowData
{
	int Number;
	FString Name;
	class UCWeaponAsset* Asset;

	FWeaponRowData()
	{

	}

	FWeaponRowData(int32 InNumber, FString InName, class UCWeaponAsset* InAsset)
		: Number(InNumber), Name(InName), Asset(InAsset)
	{

	}

	static TSharedPtr<FWeaponRowData> Make(int32 InNumber, FString InName, class UCWeaponAsset* InAsset)
	{
		return MakeShareable(new FWeaponRowData(InNumber, InName, InAsset));
	}
};
typedef TSharedPtr<FWeaponRowData> FWeaponRowDataPtr;//���� ����� �ڷ���


//���ٿ� ������ SMultiColumnTableRow

class WEAPON_API SWeaponTableRow
	: public SMultiColumnTableRow<FWeaponRowDataPtr>
{
public:
	SLATE_BEGIN_ARGS(SWeaponTableRow) { }//{ }�� �ʱ�ȭ����
	SLATE_ARGUMENT(FWeaponRowDataPtr, RowData)
		SLATE_END_ARGS()

public:
	void Construct(const FArguments& InArgs, const TSharedRef<STableViewBase>& InOwnerTable);

protected:
	TSharedRef<SWidget> GenerateWidgetForColumn(const FName& InColumnName) override;//STableRow.h���� ��ӹ޾� �������̵�.

private:
	FWeaponRowDataPtr Data;//������ �޾Ƽ� �� �ڷ��� FWeaponRowDataPtr�� ���� Data.

};


//�̺�Ʈ�� ��������Ʈ. FWeaponRowDataPtr�� �������� ȣ��.
DECLARE_DELEGATE_OneParam(FOnWeaponListViewSelectedItem, FWeaponRowDataPtr)

class WEAPON_API SWeaponLeftArea
	: public SCompoundWidget
{
public:
	SLATE_BEGIN_ARGS(SWeaponLeftArea) {}
	SLATE_EVENT(FOnWeaponListViewSelectedItem, OnSelectedItem)
		SLATE_END_ARGS()

public:
	void Construct(const FArguments& InArgs);

public:
	bool HasRowDataPtr() { return  RowDatas.Num() > 0; }//�ϳ��� ������ �ִ��� üũ
	FWeaponRowDataPtr GetFirstDataPtr() { return RowDatas[0]; }//�� ���� ������ ����

	void SelectDataPtr(class UCWeaponAsset* InAsset);

private:
	TSharedRef<ITableRow> OnGenerateRow(FWeaponRowDataPtr InRow, const TSharedRef<STableViewBase>& InTable);
	void OnSelectionChanged(FWeaponRowDataPtr InDataPtr, ESelectInfo::Type InType);

	FText OnGetAssetCount() const;

	void OnTextChanged(const FText& InText);//�Է��� �ؽ�Ʈ�� ���� �ٲ��.
	void OnTextCommitted(const FText& InText, ETextCommit::Type InType);//�Է¿Ϸ�(enter ��)������ �� ȣ��Ǵ� �̺�Ʈ.

private:
	void ReadDataAssetList();

private:
	FOnWeaponListViewSelectedItem OnListViewSelectedItem;

private:
	TArray<FWeaponRowDataPtr> RowDatas;//������ �ڷ����� �޴� �迭���� 
	TSharedPtr<SListView<FWeaponRowDataPtr>> ListView;

private:
	TSharedPtr<class SSearchBox> SearchBox;
	FText SearchText;//�˻� ���ڿ��� ������ ����
};