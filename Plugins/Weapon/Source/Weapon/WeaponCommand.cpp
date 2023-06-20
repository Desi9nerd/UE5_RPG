#include "WeaponCommand.h"
#include "WeaponStyle.h"
#include "WeaponAssetEditor.h"
#include "LevelEditor.h"
//#include "ToolMenus.h"

FWeaponCommand::FWeaponCommand()
	: TCommands("Toolbar_Buttons", FText::FromString(""), NAME_None, FEditorStyle::GetStyleSetName())
{
	Command = MakeShareable(new FUICommandList());
}

FWeaponCommand::~FWeaponCommand()
{
	if (Command.IsValid())
		Command.Reset();

	if (Extender.IsValid())
		Extender.Reset();
}

void FWeaponCommand::Startup()
{
	FWeaponCommand::RegisterCommands();
	
	Extender = MakeShareable(new FExtender());

	FToolBarExtensionDelegate toolbar = FToolBarExtensionDelegate::CreateRaw(this, &FWeaponCommand::AddToolBar);
	Extender->AddToolBarExtension("PluginTools", EExtensionHook::After, Command, toolbar);//Settings, PluginTools, Contents
	
	FLevelEditorModule& levelEditor = FModuleManager::LoadModuleChecked<FLevelEditorModule>("LevelEditor");
	levelEditor.GetToolBarExtensibilityManager()->AddExtender(Extender);
		
	/*
	//2023.06.20 화요일
	//Extender = MakeShareable(new FExtender);	
	//Extender->AddToolBarExtension("PluginTools", EExtensionHook::After, Command, FToolBarExtensionDelegate::CreateRaw(this, //&FWeaponCommand::AddToolBar));
	//
	//FToolMenuOwnerScoped OwnerScoped(this);
	//
	//{
	//	UToolMenu* Menu = UToolMenus::Get()->ExtendMenu("LevelEditor.MainMenu.Window");
	//	{
	//		FToolMenuSection& Section = Menu->FindOrAddSection("WindowLayout");
	//		Section.AddMenuEntryWithCommandList(Id, Command);
	//	}
	//}
	//{
	//	UToolMenu* ToolbarMenu = UToolMenus::Get()->ExtendMenu("LevelEditor.LevelEditorToolBar.PlayToolBar");
	//	{
	//		FToolMenuSection& Section = ToolbarMenu->FindOrAddSection("PluginTools");
	//		{
	//			FToolMenuEntry& Entry = Section.AddEntry(FToolMenuEntry::InitToolBarButton(Id));
	//			Entry.SetCommandList(Command);
	//		}
	//	}
	//}
	*/
}

void FWeaponCommand::RegisterCommands()
{
#define LOCTEXT_NAMESPACE ""
	UI_COMMAND(Id, "Weapon", "", EUserInterfaceActionType::Button, FInputChord());
#undef LOCTEXT_NAMESPACE


	FExecuteAction action;
	action.BindRaw(this, &FWeaponCommand::OnClicked);

	Command->MapAction(Id, action, FCanExecuteAction());
}

void FWeaponCommand::AddToolBar(FToolBarBuilder& InBuilder)
{
	FString name = TEXT("WeaponPlugin");//웨폰

	InBuilder.AddSeparator();
	InBuilder.AddToolBarButton(Id, NAME_None, FText::FromString(name), FText::FromString("Weapon Asset Editor"), FWeaponStyle::Get()->ToolBar_Icon, NAME_None);
}

void FWeaponCommand::OnClicked()
{
	FWeaponAssetEditor::OpenWindow();
}