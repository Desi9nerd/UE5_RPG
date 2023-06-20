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
	//2023.06.20 ȭ����
	//TSharedPtr<FToolMenuSection> Section;

	TSharedPtr<FUICommandList> Command;
	TSharedPtr<FUICommandInfo> Id;
		
private:
	void AddToolBar(FToolBarBuilder& InBuilder);//���ٿ� ��ư �߰�
	void OnClicked();//��ư�� ��������
};
