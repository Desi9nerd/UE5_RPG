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
	virtual void RegisterCommands() override;

private:
	void AddToolBar(FToolBarBuilder& InBuilder);//���ٿ� ��ư �߰�
	void OnClicked();//��ư�� ��������

	TSharedPtr<FExtender> Extender;

	TSharedPtr<FUICommandList> Command;
	TSharedPtr<FUICommandInfo> Id;
};
