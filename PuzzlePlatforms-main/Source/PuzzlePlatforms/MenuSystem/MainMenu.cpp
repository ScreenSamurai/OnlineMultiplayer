// Fill out your copyright notice in the Description page of Project Settings.


#include "MainMenu.h"
#include "Components/Button.h"
#include "Components/Widget.h"
#include "UObject/ConstructorHelpers.h"
#include "GameFramework/PlayerController.h"
#include "Components/WidgetSwitcher.h"
#include "Components/EditableTextBox.h"
#include "Components/TextBlock.h"

#include "ServerRow.h"

UMeinManu::UMeinManu(const FObjectInitializer& ObjectInitializer)
{
	ConstructorHelpers::FClassFinder<UUserWidget> ServerRowBPClass(TEXT("/Game/MenuSystem/WBP_ServerRow"));
	if (!ensure(ServerRowBPClass.Class != nullptr)) return;

	ServerRowClass = ServerRowBPClass.Class;
}

bool UMeinManu::Initialize()
{
	bool Success = Super::Initialize();
	if (!Success)return false;

	if (!ensure(HostButton != nullptr))return false;
	HostButton->OnClicked.AddDynamic(this, &UMeinManu::HostServer);

	if (!ensure(JoinServerButton != nullptr))return false;
	JoinServerButton->OnClicked.AddDynamic(this, &UMeinManu::JoinServer);

	if (!ensure(JoinButton != nullptr))return false;
	JoinButton->OnClicked.AddDynamic(this, &UMeinManu::OpenJoinMenu);

	if (!ensure(CancelButton != nullptr))return false;
	CancelButton->OnClicked.AddDynamic(this, &UMeinManu::CancelMenu);

	if (!ensure(QuitButton != nullptr))return false;
	QuitButton->OnClicked.AddDynamic(this, &UMeinManu::QuitGame);


	return true;
}

void UMeinManu::HostServer()
{
	if (InMenuInterface != nullptr)
	{
		InMenuInterface->Host();
	}
}

void UMeinManu::JoinServer()
{
	if (SelectedIndex.IsSet() && InMenuInterface != nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("Select index %d."), SelectedIndex.GetValue());
		InMenuInterface->Join(SelectedIndex.GetValue());
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Selected index not set."));
	}
}

void UMeinManu::SelectIndex(uint32 Index)
{
	SelectedIndex = Index;
	UpdateChildren();
}

void UMeinManu::UpdateChildren()
{
	for (int32 i = 0; i < ServerList->GetChildrenCount(); ++i)
	{
		auto Row = Cast<UServerRow>(ServerList->GetChildAt(i));
		if (Row != nullptr)
		{
			Row->SelectedButton = (SelectedIndex.IsSet() && SelectedIndex.GetValue() == i);
		}
	}
}

void UMeinManu::SetServerList(TArray<FServerData> ServerNames)
{
	UWorld* World = this->GetWorld();
	if (!ensure(World != nullptr)) return;

	ServerList->ClearChildren();

	uint32 i = 0;
	for (const FServerData& FServerData : ServerNames)
	{
		UServerRow* Row = CreateWidget<UServerRow>(World, ServerRowClass);
		if (!ensure(Row != nullptr)) return;

		FString FractionText = FString::Printf(TEXT("%d/%d"), FServerData.CurrentPlayers, FServerData.MaxPlayers);

		Row->ConnectionFraction->SetText(FText::FromString(FractionText));
		Row->HostUser->SetText(FText::FromString(FServerData.HostUsername));
		Row->ServerName->SetText(FText::FromString(FServerData.Name));
		Row->Setup(this, i);
		++i;
		ServerList->AddChild(Row);
	}
}

void UMeinManu::OpenJoinMenu()
{
	if (!ensure(MenuSwitcher != nullptr))return ;
	if (!ensure(JoinMenu != nullptr))return;
	MenuSwitcher->SetActiveWidgetIndex(1);
	if (InMenuInterface != nullptr)
	{
		InMenuInterface->RefreshServerList();
	}
}

void UMeinManu::CancelMenu()
{
	if (!ensure(MenuSwitcher != nullptr))return;
	if (!ensure(JoinMenu != nullptr))return;
	MenuSwitcher->SetActiveWidgetIndex(0);
}

void UMeinManu::QuitGame()
{
	UWorld* World = GetWorld();
	if (!ensure(World != nullptr)) return;

	APlayerController* PlayerController = World->GetFirstPlayerController();
	if (!ensure(PlayerController != nullptr)) return;

	PlayerController->ConsoleCommand("Quit");
}
