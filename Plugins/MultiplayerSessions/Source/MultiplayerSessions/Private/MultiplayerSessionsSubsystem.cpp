#include "MultiplayerSessionsSubsystem.h"
#include "OnlineSubsystem.h"
#include "OnlineSessionSettings.h"

UMultiplayerSessionsSubsystem::UMultiplayerSessionsSubsystem(): //Delegate�� ����� On~�Լ��� �����Ѵ�.
	CreateSessionCompleteDelegate(FOnCreateSessionCompleteDelegate::CreateUObject(this, &ThisClass::OnCreateSessionComplete)),
	FindSessionsCompleteDelegate(FOnFindSessionsCompleteDelegate::CreateUObject(this, &ThisClass::OnFindSessionsComplete)),
	JoinSessionCompleteDelegate(FOnJoinSessionCompleteDelegate::CreateUObject(this, &ThisClass::OnJoinSessionComplete)),
	DestroySessionCompleteDelegate(FOnDestroySessionCompleteDelegate::CreateUObject(this, &ThisClass::OnDestroySessionComplete)),
	StartSessionCompleteDelegate(FOnStartSessionCompleteDelegate::CreateUObject(this, &ThisClass::OnStartSessionComplete))
{
	IOnlineSubsystem* Subsystem = IOnlineSubsystem::Get();
	if (Subsystem)
	{
		SessionInterface = Subsystem->GetSessionInterface();//SessionInterface ������ IOnlineSubsystem�� SessionInterface ������ ��´�.
	}
}

void UMultiplayerSessionsSubsystem::CreateSession(int32 NumPublicConnections, FString MatchType)
{
	if (!SessionInterface.IsValid())
	{
		return;
	}

	//�̹� �����ϴ� Session�� �ִٸ� �������ش�.
	auto ExistingSession = SessionInterface->GetNamedSession(NAME_GameSession);
	if (ExistingSession != nullptr)
	{
		//Session�� �̹� �����Ͽ� Session ������ ���������� DestroySession()�� �θ��� Session�� �����Ѵ�. �̶� �����ϴ� Session�� �������� LastNumPublicConnections�� LastMatchType ������ ��´�.
		bCreateSessionOnDestroy = true;
		LastNumPublicConnections = NumPublicConnections;
		LastMatchType = MatchType;

		DestroySession();
	}

	// ���� ����� Session�� �������ش�.
	// FDelegateHandle ���� delegate�� �����Ѵ�. �׸��� ���Ŀ� Delegate List���� �����Ѵ�.
	// Store the delegate in a FDelegateHandle so we can later remove it from the delegate list
	CreateSessionCompleteDelegateHandle = SessionInterface->AddOnCreateSessionCompleteDelegate_Handle(CreateSessionCompleteDelegate);

	LastSessionSettings = MakeShareable(new FOnlineSessionSettings());
	LastSessionSettings->bIsLANMatch = IOnlineSubsystem::Get()->GetSubsystemName() == "NULL" ? true : false;//Steam Subsystem�� �����ϴ� ���� LAN Match�� �ƴϴ�. NULL Subsystem�� ����ϴ� ��� LAN Match��.
	LastSessionSettings->NumPublicConnections = NumPublicConnections;//�Լ� Input���� ����. ���� ������ Players �� ����
	LastSessionSettings->bAllowJoinInProgress = true;//Session�� on going�� �� ������ �� �ְ� ����. Session�� �������� �� Player���� ���� �� ���� �� ����
	LastSessionSettings->bAllowJoinViaPresence = true;//Steam ���ؿ��� ���� region�� �ִ� Player�鸸 ���� �� �ְ� ����.
	LastSessionSettings->bShouldAdvertise = true;//Steam���� Advertise�� �����Ͽ� ������� ã�� ���� �� �ְ� �Ѵ�.
	LastSessionSettings->bUsesPresence = true;//���� region���� session�� ã�� �� �ְ� ���ش�.
	LastSessionSettings->Set(FName("MatchType"), MatchType, EOnlineDataAdvertisementType::ViaOnlineServiceAndPing);
	LastSessionSettings->BuildUniqueId = 1;
	LastSessionSettings->bUseLobbiesIfAvailable = true;//Unreal 5.0�̻� �������� �ʿ��� �ڵ�.

	const ULocalPlayer* LocalPlayer = GetWorld()->GetFirstLocalPlayerFromController();
	if (false == SessionInterface->CreateSession(*LocalPlayer->GetPreferredUniqueNetId(), NAME_GameSession, *LastSessionSettings))//CreateSession ���ϰ��� false���(=Session ����⿡ �����Ѵٸ�)
	{
		//Delegate Handle�� ����Ͽ� Delegate List���� �����Ѵ�.
		SessionInterface->ClearOnCreateSessionCompleteDelegate_Handle(CreateSessionCompleteDelegateHandle);

		// ������� ���� MultiplayerOnCreateSessionComplete delegate�� Broadcast ���ش�.
		// Broadcast our own custom delegate
		MultiplayerOnCreateSessionComplete.Broadcast(false);//���� if���ǿ����� false
	}
}

void UMultiplayerSessionsSubsystem::FindSessions(int32 MaxSearchResults)
{
	if (false == SessionInterface.IsValid())
	{
		return;
	}

	//MultiplayerSessionsSubsystem.h ���� FindSessionsCompleteDelegate�� �����Ų��.
	FindSessionsCompleteDelegateHandle = SessionInterface->AddOnFindSessionsCompleteDelegate_Handle(FindSessionsCompleteDelegate);

	LastSessionSearch = MakeShareable(new FOnlineSessionSearch());//����
	LastSessionSearch->MaxSearchResults = MaxSearchResults;//DevID ã�� �ִ� ����.
	LastSessionSearch->bIsLanQuery = IOnlineSubsystem::Get()->GetSubsystemName() == "NULL" ? true : false;//Steam Subsystem�� �����ϴ� ���� LAN Match�� �ƴϴ�. NULL Subsystem�� ����ϴ� ��� LAN Match��.
	LastSessionSearch->QuerySettings.Set(SEARCH_PRESENCE, true, EOnlineComparisonOp::Equals);

	const ULocalPlayer* LocalPlayer = GetWorld()->GetFirstLocalPlayerFromController();
	if (false == SessionInterface->FindSessions(*LocalPlayer->GetPreferredUniqueNetId(), LastSessionSearch.ToSharedRef()))//FindSession ���ϰ��� false���(=Session ã�⿡ �����ߴٸ�)
	{
		//Delegate Handle�� ����Ͽ� FindSessionsCompleteDelegateHandle�� Delegate List���� �����Ѵ�.
		SessionInterface->ClearOnFindSessionsCompleteDelegate_Handle(FindSessionsCompleteDelegateHandle);

		//������� ���� MultiplayerOnFindSessionComplete delegate�� Broadcast ���ش�.
		MultiplayerOnFindSessionsComplete.Broadcast(TArray<FOnlineSessionSearchResult>(), false);//menu�� �� Broadcast�� ���� �� false�� ������ FOnlineSessionSearchResult �迭�� ����ִٴ� ���� �� �� �ִ�.
	}
}

void UMultiplayerSessionsSubsystem::JoinSession(const FOnlineSessionSearchResult& SessionResult)
{
	if (false == SessionInterface.IsValid())//SessionInterface�� ���ٸ�
	{
		//Menu�� UnknownError���(=JoinSession �� �� ���ٰ�) �˷��ش�. 
		MultiplayerOnJoinSessionComplete.Broadcast(EOnJoinSessionCompleteResult::UnknownError);
		return;
	}

	JoinSessionCompleteDelegateHandle = SessionInterface->AddOnJoinSessionCompleteDelegate_Handle(JoinSessionCompleteDelegate);

	const ULocalPlayer* LocalPlayer = GetWorld()->GetFirstLocalPlayerFromController();
	if (false == SessionInterface->JoinSession(*LocalPlayer->GetPreferredUniqueNetId(), NAME_GameSession, SessionResult))//JoinSession ���ϰ��� false���(=Session Join�� �����ߴٸ�)
	{
		SessionInterface->ClearOnJoinSessionCompleteDelegate_Handle(JoinSessionCompleteDelegateHandle);

		MultiplayerOnJoinSessionComplete.Broadcast(EOnJoinSessionCompleteResult::UnknownError);
	}
}

void UMultiplayerSessionsSubsystem::DestroySession()
{
	if (false == SessionInterface.IsValid())
	{
		MultiplayerOnDestroySessionComplete.Broadcast(false);
		return;
	}

	//Delegate ����.
	DestroySessionCompleteDelegateHandle = SessionInterface->AddOnDestroySessionCompleteDelegate_Handle(DestroySessionCompleteDelegate);

	//DestroySession�� ������ ���
	if (false == SessionInterface->DestroySession(NAME_GameSession))
	{
		SessionInterface->ClearOnDestroySessionCompleteDelegate_Handle(DestroySessionCompleteDelegateHandle);
		MultiplayerOnDestroySessionComplete.Broadcast(false);
	}
}

void UMultiplayerSessionsSubsystem::StartSession()
{
}

void UMultiplayerSessionsSubsystem::OnCreateSessionComplete(FName SessionName, bool bWasSuccessful)
{
	//SessionInterfaces�� Delegate�� �����ش�.
	if (SessionInterface)
	{
		SessionInterface->ClearOnCreateSessionCompleteDelegate_Handle(CreateSessionCompleteDelegateHandle);
	}
	//MultiplayerOnCreateSessionComplete delegate�� Broadcast ���ش�. �̷����ϸ� Menu Ŭ������ callback function ������ �ȴ�.
	MultiplayerOnCreateSessionComplete.Broadcast(bWasSuccessful);
}

//Session�� ���������� ã�� ��� �� �Ǵ� �Լ� 
void UMultiplayerSessionsSubsystem::OnFindSessionsComplete(bool bWasSuccessful)
{
	if (SessionInterface)
	{
		SessionInterface->ClearOnFindSessionsCompleteDelegate_Handle(FindSessionsCompleteDelegateHandle);
	}

	if (LastSessionSearch->SearchResults.Num() <= 0)//SearchResults �迭 ������� ��ȸ���� ã���� ��, ã������ 0���϶��(=�ƹ��͵� ã�� ���ߴٸ�)
	{
		MultiplayerOnFindSessionsComplete.Broadcast(TArray<FOnlineSessionSearchResult>(), false);//menu�� �� Broadcast�� ���� �� false�� ������ FOnlineSessionSearchResult �迭�� ����ִٴ� ���� �� �� �ִ�.
		return;//���� �ڵ尡 ������� �ʵ��� ���⼭ ���� �����ش�.
	}

	MultiplayerOnFindSessionsComplete.Broadcast(LastSessionSearch->SearchResults, bWasSuccessful);
}

void UMultiplayerSessionsSubsystem::OnJoinSessionComplete(FName SessionName, EOnJoinSessionCompleteResult::Type Result)
{
	if (SessionInterface)
	{
		SessionInterface->ClearOnJoinSessionCompleteDelegate_Handle(JoinSessionCompleteDelegateHandle);
	}

	//EOnJoinSessionCompleteResult�� Broadcast ���ش�.
	MultiplayerOnJoinSessionComplete.Broadcast(Result);
}

void UMultiplayerSessionsSubsystem::OnDestroySessionComplete(FName SessionName, bool bWasSuccessful)
{
	if (SessionInterface)
	{
		SessionInterface->ClearOnDestroySessionCompleteDelegate_Handle(DestroySessionCompleteDelegateHandle);
	}
	if (bWasSuccessful && bCreateSessionOnDestroy)
	{
		bCreateSessionOnDestroy = false;
		CreateSession(LastNumPublicConnections, LastMatchType);//������ Session�� �����Ѵ�. ex. Join�ؼ� �ٸ����� Session�� �����ߴ�. Host�� Session�� ���������� Client �÷��̾��� Session���� ���ư���.
	}
	MultiplayerOnDestroySessionComplete.Broadcast(bWasSuccessful);
}

void UMultiplayerSessionsSubsystem::OnStartSessionComplete(FName SessionName, bool bWasSuccessful)
{
}