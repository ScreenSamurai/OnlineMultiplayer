// Fill out your copyright notice in the Description page of Project Settings.


#include "MainMenu.h"
#include "Components/Button.h"
#include "Components/Widget.h"
#include "GameFramework/PlayerController.h"
#include "Components/WidgetSwitcher.h"
#include "Components/EditableTextBox.h"


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
	if (InMenuInterface != nullptr)
	{
		if (!ensure(IPAddresField != nullptr))return;
		InMenuInterface->Join(IPAddresField->GetText().ToString());
	}
}

void UMeinManu::OpenJoinMenu()
{
	if (!ensure(MenuSwitcher != nullptr))return ;
	if (!ensure(JoinMenu != nullptr))return;
	MenuSwitcher->SetActiveWidgetIndex(1);
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
