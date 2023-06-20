#pragma once

#include "CoreMinimal.h"
#include "Framework/Commands/Commands.h"

class FWeaponCommand
	: public TCommands<FWeaponCommand>
{
public:
	FWeaponCommand();
	~FWeaponCommand();

	void Startup();

public:
	virtual void RegisterCommands() override;

private:
	TSharedPtr<FExtender> Extender;
	//2023.06.20 화요일
	//TSharedPtr<FToolMenuSection> Section;

	TSharedPtr<FUICommandList> Command;
	TSharedPtr<FUICommandInfo> Id;
		
private:
	void AddToolBar(FToolBarBuilder& InBuilder);//툴바에 버튼 추가
	void OnClicked();//버튼이 눌렸을때
};
