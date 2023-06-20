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
typedef TSharedPtr<FWeaponRowData> FWeaponRowDataPtr;//실제 출력할 자료형


//한줄에 여러개 SMultiColumnTableRow

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
	TSharedRef<SWidget> GenerateWidgetForColumn(const FName& InColumnName) override;//STableRow.h에서 상속받아 오버라이드.

private:
	FWeaponRowDataPtr Data;//실제로 받아서 쓸 자료형 FWeaponRowDataPtr의 변수 Data.

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
	TArray<FWeaponRowDataPtr> RowDatas;//데이터 자료형을 받는 배열변수 
	TSharedPtr<SListView<FWeaponRowDataPtr>> ListView;
};