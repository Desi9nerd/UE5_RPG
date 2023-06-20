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
	SLATE_BEGIN_ARGS(SWeaponTableRow) { }//{ }는 초기화영역
	SLATE_ARGUMENT(FWeaponRowDataPtr, RowData)
		SLATE_END_ARGS()

public:
	void Construct(const FArguments& InArgs, const TSharedRef<STableViewBase>& InOwnerTable);

protected:
	TSharedRef<SWidget> GenerateWidgetForColumn(const FName& InColumnName) override;//STableRow.h에서 상속받아 오버라이드.

private:
	FWeaponRowDataPtr Data;//실제로 받아서 쓸 자료형 FWeaponRowDataPtr의 변수 Data.

};


//이벤트용 델리게이트. FWeaponRowDataPtr이 눌렸을때 호출.
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
	bool HasRowDataPtr() { return  RowDatas.Num() > 0; }//하나라도 데이터 있는지 체크
	FWeaponRowDataPtr GetFirstDataPtr() { return RowDatas[0]; }//맨 위의 데이터 리턴

	void SelectDataPtr(class UCWeaponAsset* InAsset);

private:
	TSharedRef<ITableRow> OnGenerateRow(FWeaponRowDataPtr InRow, const TSharedRef<STableViewBase>& InTable);
	void OnSelectionChanged(FWeaponRowDataPtr InDataPtr, ESelectInfo::Type InType);

	FText OnGetAssetCount() const;

	void OnTextChanged(const FText& InText);//입력한 텍스트가 들어와 바뀐다.
	void OnTextCommitted(const FText& InText, ETextCommit::Type InType);//입력완료(enter 등)시켰을 때 호출되는 이벤트.

private:
	void ReadDataAssetList();

private:
	FOnWeaponListViewSelectedItem OnListViewSelectedItem;

private:
	TArray<FWeaponRowDataPtr> RowDatas;//데이터 자료형을 받는 배열변수 
	TSharedPtr<SListView<FWeaponRowDataPtr>> ListView;

private:
	TSharedPtr<class SSearchBox> SearchBox;
	FText SearchText;//검색 문자열을 관리할 변수
};