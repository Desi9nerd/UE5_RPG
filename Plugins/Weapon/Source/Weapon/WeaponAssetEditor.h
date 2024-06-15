#pragma once
#include "CoreMinimal.h"
#include "SWeaponLeftArea.h"
#include "Toolkits/AssetEditorToolkit.h"

class WEAPON_API FWeaponAssetEditor
	: public FAssetEditorToolkit
{
public:
	static void OpenWindow(FString InAssetName = "");
	static void Shutdown();
	void RegisterTabSpawners(const TSharedRef<FTabManager>& InTabManager) override;
	
	FName GetToolkitFName() const override;
	FText GetBaseToolkitName() const override;
	FString GetWorldCentricTabPrefix() const override;
	FLinearColor GetWorldCentricTabColorScale() const override;

protected:
	bool OnRequestClose() override;//â�� ���� �� ���� �ǰ� ����.

private:
	static TSharedPtr<FWeaponAssetEditor> Instance;
	void Open(FString InAssetName);
	
	TSharedRef<SDockTab> Spawn_LeftAreaTab(const FSpawnTabArgs& InArgs);
	TSharedRef<SDockTab> Spawn_DetailsViewTab(const FSpawnTabArgs& InArgs);
	
	void OnListViewSelectedItem(FWeaponRowDataPtr InDataPtr);//LeftArea���� ������ FWeaponRowDataPtr
	FReply OnClicked();

	TSharedPtr<class SWeaponLeftArea> LeftArea;
	TSharedPtr<class IDetailsView> DetailsView;//IDetailsView�� DetailView�� �ֻ��� �ڷ���.
	
	static const FName EditorName;
	static const FName LeftAreaTabId;
	static const FName DetailTabId;
};