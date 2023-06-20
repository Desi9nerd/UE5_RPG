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
	SLATE_BEGIN_ARGS(SWeaponTableRow) {}
	SLATE_ARGUMENT(FWeaponRowDataPtr, RowData)
		SLATE_END_ARGS()

public:
	void Construct(const FArguments& InArgs, const TSharedRef<STableViewBase>& InOwnerTable);

protected:
	TSharedRef<SWidget> GenerateWidgetForColumn(const FName& InColumnName) override;//STableRow.h���� ��ӹ޾� �������̵�.

private:
	FWeaponRowDataPtr Data;//������ �޾Ƽ� �� �ڷ��� FWeaponRowDataPtr�� ���� Data.

};


class WEAPON_API SWeaponLeftArea
	: public SCompoundWidget
{
public:
	SLATE_BEGIN_ARGS(SWeaponLeftArea) {}
	SLATE_END_ARGS()

public:
	void Construct(const FArguments& InArgs);

private:
	TSharedRef<ITableRow> OnGenerateRow(FWeaponRowDataPtr InRow, const TSharedRef<STableViewBase>& InTable);

	FText OnGetAssetCount() const;

private:
	void ReadDataAssetList();

private:
	TArray<FWeaponRowDataPtr> RowDatas;//������ �ڷ����� �޴� �迭���� 
	TSharedPtr<SListView<FWeaponRowDataPtr>> ListView;
};