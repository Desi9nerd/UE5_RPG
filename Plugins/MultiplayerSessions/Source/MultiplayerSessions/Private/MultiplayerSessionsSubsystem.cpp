#include "MultiplayerSessionsSubsystem.h"
#include "OnlineSubsystem.h"
#include "OnlineSessionSettings.h"

UMultiplayerSessionsSubsystem::UMultiplayerSessionsSubsystem(): //Delegate을 만들어 On~함수에 연결한다.
	CreateSessionCompleteDelegate(FOnCreateSessionCompleteDelegate::CreateUObject(this, &ThisClass::OnCreateSessionComplete)),
	FindSessionsCompleteDelegate(FOnFindSessionsCompleteDelegate::CreateUObject(this, &ThisClass::OnFindSessionsComplete)),
	JoinSessionCompleteDelegate(FOnJoinSessionCompleteDelegate::CreateUObject(this, &ThisClass::OnJoinSessionComplete)),
	DestroySessionCompleteDelegate(FOnDestroySessionCompleteDelegate::CreateUObject(this, &ThisClass::OnDestroySessionComplete)),
	StartSessionCompleteDelegate(FOnStartSessionCompleteDelegate::CreateUObject(this, &ThisClass::OnStartSessionComplete))
{
	IOnlineSubsystem* Subsystem = IOnlineSubsystem::Get();
	if (Subsystem)
	{
		SessionInterface = Subsystem->GetSessionInterface();//SessionInterface 변수에 IOnlineSubsystem의 SessionInterface 정보를 담는다.
	}
}

void UMultiplayerSessionsSubsystem::CreateSession(int32 NumPublicConnections, FString MatchType)
{
	if (!SessionInterface.IsValid())
	{
		return;
	}

	//이미 존재하는 Session이 있다면 제거해준다.
	auto ExistingSession = SessionInterface->GetNamedSession(NAME_GameSession);
	if (ExistingSession != nullptr)
	{
		//Session이 이미 존재하여 Session 생성을 실패했을시 DestroySession()을 부르고 Session을 생성한다. 이때 존재하는 Session의 정보들을 LastNumPublicConnections와 LastMatchType 변수에 담는다.
		bCreateSessionOnDestroy = true;
		LastNumPublicConnections = NumPublicConnections;
		LastMatchType = MatchType;

		DestroySession();
	}

	// 새로 만드는 Session을 세팅해준다.
	// FDelegateHandle 내에 delegate을 저장한다. 그리고 추후에 Delegate List에서 제거한다.
	// Store the delegate in a FDelegateHandle so we can later remove it from the delegate list
	CreateSessionCompleteDelegateHandle = SessionInterface->AddOnCreateSessionCompleteDelegate_Handle(CreateSessionCompleteDelegate);

	LastSessionSettings = MakeShareable(new FOnlineSessionSettings());
	LastSessionSettings->bIsLANMatch = IOnlineSubsystem::Get()->GetSubsystemName() == "NULL" ? true : false;//Steam Subsystem에 연결하는 경우는 LAN Match가 아니다. NULL Subsystem을 사용하는 경우 LAN Match다.
	LastSessionSettings->NumPublicConnections = NumPublicConnections;//함수 Input값을 적용. 접속 가능한 Players 수 제한
	LastSessionSettings->bAllowJoinInProgress = true;//Session이 on going일 때 참가할 수 있게 설정. Session이 실행중일 때 Player들이 원할 때 들어올 수 있음
	LastSessionSettings->bAllowJoinViaPresence = true;//Steam 기준에서 같은 region에 있는 Player들만 들어올 수 있게 설정.
	LastSessionSettings->bShouldAdvertise = true;//Steam에서 Advertise가 가능하여 사람들이 찾아 들어올 수 있게 한다.
	LastSessionSettings->bUsesPresence = true;//같은 region에서 session을 찾을 수 있게 해준다.
	LastSessionSettings->Set(FName("MatchType"), MatchType, EOnlineDataAdvertisementType::ViaOnlineServiceAndPing);
	LastSessionSettings->BuildUniqueId = 1;
	LastSessionSettings->bUseLobbiesIfAvailable = true;//Unreal 5.0이상 버젼에서 필요한 코드.

	const ULocalPlayer* LocalPlayer = GetWorld()->GetFirstLocalPlayerFromController();
	if (false == SessionInterface->CreateSession(*LocalPlayer->GetPreferredUniqueNetId(), NAME_GameSession, *LastSessionSettings))//CreateSession 리턴값이 false라면(=Session 만들기에 실패한다면)
	{
		//Delegate Handle를 사용하여 Delegate List에서 제거한다.
		SessionInterface->ClearOnCreateSessionCompleteDelegate_Handle(CreateSessionCompleteDelegateHandle);

		// 헤더에서 만든 MultiplayerOnCreateSessionComplete delegate을 Broadcast 해준다.
		// Broadcast our own custom delegate
		MultiplayerOnCreateSessionComplete.Broadcast(false);//위의 if조건에서는 false
	}
}

void UMultiplayerSessionsSubsystem::FindSessions(int32 MaxSearchResults)
{
	if (false == SessionInterface.IsValid())
	{
		return;
	}

	//MultiplayerSessionsSubsystem.h 내의 FindSessionsCompleteDelegate을 연결시킨다.
	FindSessionsCompleteDelegateHandle = SessionInterface->AddOnFindSessionsCompleteDelegate_Handle(FindSessionsCompleteDelegate);

	LastSessionSearch = MakeShareable(new FOnlineSessionSearch());//생성
	LastSessionSearch->MaxSearchResults = MaxSearchResults;//DevID 찾기 최대 개수.
	LastSessionSearch->bIsLanQuery = IOnlineSubsystem::Get()->GetSubsystemName() == "NULL" ? true : false;//Steam Subsystem에 연결하는 경우는 LAN Match가 아니다. NULL Subsystem을 사용하는 경우 LAN Match다.
	LastSessionSearch->QuerySettings.Set(SEARCH_PRESENCE, true, EOnlineComparisonOp::Equals);

	const ULocalPlayer* LocalPlayer = GetWorld()->GetFirstLocalPlayerFromController();
	if (false == SessionInterface->FindSessions(*LocalPlayer->GetPreferredUniqueNetId(), LastSessionSearch.ToSharedRef()))//FindSession 리턴값이 false라면(=Session 찾기에 실패했다면)
	{
		//Delegate Handle를 사용하여 FindSessionsCompleteDelegateHandle을 Delegate List에서 제거한다.
		SessionInterface->ClearOnFindSessionsCompleteDelegate_Handle(FindSessionsCompleteDelegateHandle);

		//헤더에서 만든 MultiplayerOnFindSessionComplete delegate을 Broadcast 해준다.
		MultiplayerOnFindSessionsComplete.Broadcast(TArray<FOnlineSessionSearchResult>(), false);//menu가 이 Broadcast를 받을 때 false로 받으면 FOnlineSessionSearchResult 배열이 비어있다는 것을 알 수 있다.
	}
}

void UMultiplayerSessionsSubsystem::JoinSession(const FOnlineSessionSearchResult& SessionResult)
{
	if (false == SessionInterface.IsValid())//SessionInterface가 없다면
	{
		//Menu에 UnknownError라고(=JoinSession 할 수 없다고) 알려준다. 
		MultiplayerOnJoinSessionComplete.Broadcast(EOnJoinSessionCompleteResult::UnknownError);
		return;
	}

	JoinSessionCompleteDelegateHandle = SessionInterface->AddOnJoinSessionCompleteDelegate_Handle(JoinSessionCompleteDelegate);

	const ULocalPlayer* LocalPlayer = GetWorld()->GetFirstLocalPlayerFromController();
	if (false == SessionInterface->JoinSession(*LocalPlayer->GetPreferredUniqueNetId(), NAME_GameSession, SessionResult))//JoinSession 리턴값이 false라면(=Session Join에 실패했다면)
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

	//Delegate 연결.
	DestroySessionCompleteDelegateHandle = SessionInterface->AddOnDestroySessionCompleteDelegate_Handle(DestroySessionCompleteDelegate);

	//DestroySession이 실패한 경우
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
	//SessionInterfaces의 Delegate을 지워준다.
	if (SessionInterface)
	{
		SessionInterface->ClearOnCreateSessionCompleteDelegate_Handle(CreateSessionCompleteDelegateHandle);
	}
	//MultiplayerOnCreateSessionComplete delegate를 Broadcast 해준다. 이렇게하면 Menu 클래스는 callback function 가지게 된다.
	MultiplayerOnCreateSessionComplete.Broadcast(bWasSuccessful);
}

//Session을 성공적으로 찾은 경우 콜 되는 함수 
void UMultiplayerSessionsSubsystem::OnFindSessionsComplete(bool bWasSuccessful)
{
	if (SessionInterface)
	{
		SessionInterface->ClearOnFindSessionsCompleteDelegate_Handle(FindSessionsCompleteDelegateHandle);
	}

	if (LastSessionSearch->SearchResults.Num() <= 0)//SearchResults 배열 결과값을 순회에서 찾았을 때, 찾은것이 0이하라면(=아무것도 찾지 못했다면)
	{
		MultiplayerOnFindSessionsComplete.Broadcast(TArray<FOnlineSessionSearchResult>(), false);//menu가 이 Broadcast를 받을 때 false로 받으면 FOnlineSessionSearchResult 배열이 비어있다는 것을 알 수 있다.
		return;//밑의 코드가 진행되지 않도록 여기서 리턴 시켜준다.
	}

	MultiplayerOnFindSessionsComplete.Broadcast(LastSessionSearch->SearchResults, bWasSuccessful);
}

void UMultiplayerSessionsSubsystem::OnJoinSessionComplete(FName SessionName, EOnJoinSessionCompleteResult::Type Result)
{
	if (SessionInterface)
	{
		SessionInterface->ClearOnJoinSessionCompleteDelegate_Handle(JoinSessionCompleteDelegateHandle);
	}

	//EOnJoinSessionCompleteResult를 Broadcast 해준다.
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
		CreateSession(LastNumPublicConnections, LastMatchType);//기존의 Session을 생성한다. ex. Join해서 다른이의 Session에 입장했다. Host가 Session을 종료했을시 Client 플레이어의 Session으로 돌아간다.
	}
	MultiplayerOnDestroySessionComplete.Broadcast(bWasSuccessful);
}

void UMultiplayerSessionsSubsystem::OnStartSessionComplete(FName SessionName, bool bWasSuccessful)
{
}