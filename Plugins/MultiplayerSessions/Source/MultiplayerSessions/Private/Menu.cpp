#include "Menu.h"
#include "Components/Button.h"
#include "MultiplayerSessionsSubsystem.h"
#include "OnlineSessionSettings.h"
#include "OnlineSubsystem.h"

void UMenu::MenuSetup(int32 NumberOfPublicConnections, FString TypeOfMatch, FString LobbyPath)
{
	PathToLobby = FString::Printf(TEXT("%s?listen"), *LobbyPath);
	NumPublicConnections = NumberOfPublicConnections;
	MatchType = TypeOfMatch;
	AddToViewport();
	SetVisibility(ESlateVisibility::Visible);
	bIsFocusable = true;

	UWorld* World = GetWorld();
	if (World)
	{
		APlayerController* PlayerController = World->GetFirstPlayerController();
		if (PlayerController)
		{
			FInputModeUIOnly InputModeData;
			InputModeData.SetWidgetToFocus(TakeWidget());
			InputModeData.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
			PlayerController->SetInputMode(InputModeData);
			PlayerController->SetShowMouseCursor(true);
		}
	}

	UGameInstance* GameInstance = GetGameInstance();
	if (GameInstance)
	{
		//������� ���� MultiplayerSessionsSubsystem ������ Subsystem�� ���� ���� �־��ش�.
		MultiplayerSessionsSubsystem = GameInstance->GetSubsystem<UMultiplayerSessionsSubsystem>();
	}

	if (MultiplayerSessionsSubsystem)
	{
		//callback�� Bind���ش�.
		MultiplayerSessionsSubsystem->MultiplayerOnCreateSessionComplete.AddDynamic(this, &ThisClass::OnCreateSession);//Dynamic�� Bind�ϴ� ���� AddDynamic ���.
		MultiplayerSessionsSubsystem->MultiplayerOnFindSessionsComplete.AddUObject(this, &ThisClass::OnFindSessions);//Dynamic�� �ƴ� ���� Bind�ϴ� ���� AddUObject ���.
		MultiplayerSessionsSubsystem->MultiplayerOnJoinSessionComplete.AddUObject(this, &ThisClass::OnJoinSession);
		MultiplayerSessionsSubsystem->MultiplayerOnDestroySessionComplete.AddDynamic(this, &ThisClass::OnDestroySession);
		MultiplayerSessionsSubsystem->MultiplayerOnStartSessionComplete.AddDynamic(this, &ThisClass::OnStartSession);
	}
}

bool UMenu::Initialize()
{
	//Super������ false�� false�� �����Ѵ�.(=���� Ŭ�������� Initialize �ϴ°� ������ false�� �����Ͽ� Initialize()�� ���� �ʴ´�.)
	if (!Super::Initialize())
	{
		return false;
	}

	if (HostButton)
	{
		//HostButton Ŭ���� �� �߻��ϴ� �̺�Ʈ�� Delegate�� �������ش�.
		HostButton->OnClicked.AddDynamic(this, &ThisClass::HostButtonClicked);
	}
	if (JoinButton)
	{
		//JoinButton Ŭ���� �� �߻��ϴ� �̺�Ʈ�� Delegate�� �������ش�.
		JoinButton->OnClicked.AddDynamic(this, &ThisClass::JoinButtonClicked);
	}

	return true;
}

void UMenu::OnLevelRemovedFromWorld(ULevel* InLevel, UWorld* InWorld)
{
	MenuTearDown();//Widget�� ����Ʈ���� ����� input mode�� ���µȴ�.
	Super::OnLevelRemovedFromWorld(InLevel, InWorld);
}

void UMenu::OnCreateSession(bool bWasSuccessful)
{
	if (bWasSuccessful)
	{
		if (GEngine)
		{
			GEngine->AddOnScreenDebugMessage(
				-1,
				15.f,
				FColor::Yellow,
				FString(TEXT("Session created successfully!"))
			);
		}

		UWorld* World = GetWorld();
		if (World)
		{
			//World->ServerTravel("/Game/ThirdPerson/Maps/Lobby?listen");
			World->ServerTravel(PathToLobby);
		}
	}
	else//������ ���
	{
		if (GEngine)
		{
			GEngine->AddOnScreenDebugMessage(
				-1,
				15.f,
				FColor::Red,
				FString(TEXT("Failed to create session!"))
			);
		}
	}
	HostButton->SetIsEnabled(true);//������ ��� Host ��ư�� ���� �� �ֵ��� SetIsEnabled�� true�� �����.
}

void UMenu::OnFindSessions(const TArray<FOnlineSessionSearchResult>& SessionResults, bool bWasSuccessful)
{
	if (MultiplayerSessionsSubsystem == nullptr)
	{
		return;
	}

	for (auto Result : SessionResults)//������ ���� SessionResults�迭�� ��ȸ�ϸ� �˻�
	{
		FString SettingsValue;
		Result.Session.SessionSettings.Get(FName("MatchType"), SettingsValue);
		//SessionSettings�� ������� "MatchType"�� SetingsValue�� ������ �ִ��� Ȯ��.
		if (SettingsValue == MatchType)
		{
			MultiplayerSessionsSubsystem->JoinSession(Result);
			return;
		}
	}

	if (!bWasSuccessful || SessionResults.Num() == 0)
	{
		JoinButton->SetIsEnabled(true);//Session ã�⸦ ������ ��� Join ��ư�� ���� �� �ֵ��� SetIsEnabled�� true�� �����.
	}
}

void UMenu::OnJoinSession(EOnJoinSessionCompleteResult::Type Result)
{
	//IOnlineSubsystem ����� ���� ���� #include "OnlineSubsystem.h" �߰�
	IOnlineSubsystem* Subsystem = IOnlineSubsystem::Get();
	if (Subsystem)
	{
		IOnlineSessionPtr SessionInterface = Subsystem->GetSessionInterface();
		if (SessionInterface.IsValid())
		{
			FString Address;
			SessionInterface->GetResolvedConnectString(NAME_GameSession, Address);

			APlayerController* PlayerController = GetGameInstance()->GetFirstLocalPlayerController();
			if (PlayerController)
			{
				PlayerController->ClientTravel(Address, ETravelType::TRAVEL_Absolute);
			}
		}
	}
}

void UMenu::OnDestroySession(bool bWasSuccessful)
{
}

void UMenu::OnStartSession(bool bWasSuccessful)
{
}

void UMenu::HostButtonClicked()
{
	//������
	/*if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(
			-1,
			15.f,
			FColor::Yellow,
			FString(TEXT("Host Button Clicked"))
		);
	}*/

	HostButton->SetIsEnabled(false);//��ư�� ������ ���� Session�� ��� �����ϰų� ã�°��� �����ϱ� ���� ��ư�� Ŭ���ϸ� �� �̻� ������ �ʰ� SetIsEnabled�� false�� �����Ѵ�.

	if (MultiplayerSessionsSubsystem)
	{
		MultiplayerSessionsSubsystem->CreateSession(NumPublicConnections, MatchType);//NumPublicConnections ���� ũ����� Players ���� ����
		UWorld* World = GetWorld();
	}
}

void UMenu::JoinButtonClicked()
{
	//����� �׽�Ʈ
	/*if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(
			-1,
			15.f,
			FColor::Yellow,
			FString(TEXT("Join Button Clicked"))
		);
	}*/

	JoinButton->SetIsEnabled(false);//��ư�� ������ ���� Session�� ��� �����ϰų� ��� ã�°��� �����ϱ� ���� ��ư�� Ŭ���ϸ� �� �̻� ������ �ʰ� SetIsEnabled�� false�� �����Ѵ�.

	if (MultiplayerSessionsSubsystem)
	{
		//Steam DevID�� ���ڰ� Ŭ���� ������ MaxSearchResults���� 10000���� �˳��� ��´�.
		MultiplayerSessionsSubsystem->FindSessions(10000);
	}
}

void UMenu::MenuTearDown()
{
	RemoveFromParent();
	UWorld* World = GetWorld();
	if (World)
	{
		APlayerController* PlayerController = World->GetFirstPlayerController();
		if (PlayerController)
		{
			FInputModeGameOnly InputModeData;
			PlayerController->SetInputMode(InputModeData);
			PlayerController->SetShowMouseCursor(false);
		}
	}
}