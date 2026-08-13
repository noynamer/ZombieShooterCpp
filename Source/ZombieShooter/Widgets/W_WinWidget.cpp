#include "W_WinWidget.h"

#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Components/Button.h"

//------------------------------------------------------------------------------------------------------------
void UW_WinWidget::NativeConstruct ()
{
	Super::NativeConstruct();

	APlayerController* PlayerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	
	FInputModeUIOnly InputModeData;
	InputModeData.SetWidgetToFocus(TakeWidget());
	InputModeData.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);

	PlayerController->bShowMouseCursor = true;
	PlayerController->SetInputMode(InputModeData);
	
	UGameplayStatics::SetGamePaused(GetWorld(), true);

	if (ButtonAgain)
	{
		ButtonAgain->OnClicked.AddDynamic(this, &UW_WinWidget::OnClickedButtonAgain);
	}

	if (ButtonExit)
	{
		ButtonExit->OnClicked.AddDynamic(this, &UW_WinWidget::OnClickedButtonExit);
	}
}
//------------------------------------------------------------------------------------------------------------
void UW_WinWidget::OnClickedButtonAgain ()
{
	UGameplayStatics::OpenLevel(GetWorld(), DemoDayName, true);

	APlayerController* PlayerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);

	FInputModeGameOnly InputMode;

	PlayerController->bShowMouseCursor = false;
	PlayerController->SetInputMode(InputMode);
	
	UGameplayStatics::SetGamePaused(GetWorld(), false);
}
//------------------------------------------------------------------------------------------------------------
void UW_WinWidget::OnClickedButtonExit ()
{
	APlayerController* PlayerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);

	UKismetSystemLibrary::QuitGame(GetWorld(), PlayerController, EQuitPreference::Quit, false);
}
//------------------------------------------------------------------------------------------------------------