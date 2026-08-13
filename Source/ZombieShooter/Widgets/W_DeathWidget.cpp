#include "W_DeathWidget.h"

#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Components/Button.h"

//------------------------------------------------------------------------------------------------------------
void UW_DeathWidget::NativeConstruct ()
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
		ButtonAgain->OnClicked.AddDynamic(this, &UW_DeathWidget::OnClickedButtonAgain);
	}

	if (ButtonExit)
	{
		ButtonExit->OnClicked.AddDynamic(this, &UW_DeathWidget::OnClickedButtonExit);
	}
}
//------------------------------------------------------------------------------------------------------------
void UW_DeathWidget::OnClickedButtonAgain ()
{
	UGameplayStatics::OpenLevel(GetWorld(), DemoDayName, true);

	APlayerController* PlayerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);

	FInputModeGameOnly InputMode;

	PlayerController->bShowMouseCursor = false;
	PlayerController->SetInputMode(InputMode);

	UGameplayStatics::SetGamePaused(GetWorld(), false);
}
//------------------------------------------------------------------------------------------------------------
void UW_DeathWidget::OnClickedButtonExit ()
{
	APlayerController* PlayerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);

	UKismetSystemLibrary::QuitGame(GetWorld(), PlayerController, EQuitPreference::Quit, false);
}
//------------------------------------------------------------------------------------------------------------

