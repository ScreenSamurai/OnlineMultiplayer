// Fill out your copyright notice in the Description page of Project Settings.


#include "InGameMenu.h"
#include "Components/WidgetSwitcher.h"
#include "Components/Button.h"

bool UInGameMenu::Initialize()
{
	bool Success = Super::Initialize();
	if (!Success)return false;

	if (!ensure(CancelButton != nullptr))return false;
	CancelButton->OnClicked.AddDynamic(this, &UInGameMenu::CencelMenu);

	if (!ensure(ExitButton != nullptr))return false;
	ExitButton->OnClicked.AddDynamic(this, &UInGameMenu::ExitMenu);

	return true;
}

void UInGameMenu::CencelMenu()
{
	Teardown();
}

void UInGameMenu::ExitMenu()
{
	if (InMenuInterface != nullptr)
	{
		Teardown();
		InMenuInterface->JoinMainMenu();
	}
}