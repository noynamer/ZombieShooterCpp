#include "W_MainMenu.h"

#include "Kismet/GameplayStatics.h"
#include "Components/Button.h"

//------------------------------------------------------------------------------------------------------------
void UW_MainMenu::NativeConstruct ()
{
	Super::NativeConstruct();

	APlayerController* PlayerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);

	FInputModeUIOnly InputModeData;
	InputModeData.SetWidgetToFocus(TakeWidget());
	InputModeData.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);

	PlayerController->bShowMouseCursor = true;
	PlayerController->SetInputMode(InputModeData);

	if (BeginPlayButton)
	{
		BeginPlayButton->OnClicked.AddDynamic(this, &ThisClass::OnClickedButtonBeginPlay);
		//BeginPlayButton->OnHovered.AddDynamic(this, &ThisClass::OnHoveredButtonBeginPlay);
		//BeginPlayButton->OnUnhovered.AddDynamic(this, &ThisClass::OnUnhoveredButtonBeginPlay);
	}

	if (ExitButton)
	{
		ExitButton->OnClicked.AddDynamic(this, &ThisClass::OnClickedButtonExit);
		//ExitButton->OnHovered.AddDynamic(this, &ThisClass::OnHoveredButtonExit);
		//ExitButton->OnUnhovered.AddDynamic(this, &ThisClass::OnUnhoveredButtonExit);
	}
}
//------------------------------------------------------------------------------------------------------------
void UW_MainMenu::OnClickedButtonBeginPlay ()
{
	RemoveFromParent();

	UGameplayStatics::OpenLevel(GetWorld(), DemoDayName, true);

	APlayerController* PlayerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);

	FInputModeGameOnly InputMode;

	PlayerController->SetInputMode(InputMode);

	if (StartCameraClass)
	{
		StartCameraActor = UGameplayStatics::GetActorOfClass(GetWorld(), StartCameraClass);
	}

	if (MainCameraClass)
	{
		MainCameraActor = UGameplayStatics::GetActorOfClass(GetWorld(), MainCameraClass);
	}

	if (ExitCameraClass)
	{
		ExitCameraActor = UGameplayStatics::GetActorOfClass(GetWorld(), ExitCameraClass);
	}
}
//------------------------------------------------------------------------------------------------------------
void UW_MainMenu::OnHoveredButtonBeginPlay ()
{
	APlayerController* PlayerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);

	PlayerController->SetViewTargetWithBlend(StartCameraActor, 2.0f, EViewTargetBlendFunction::VTBlend_Linear, 2.0f, false);
}
//------------------------------------------------------------------------------------------------------------
void UW_MainMenu::OnUnhoveredButtonBeginPlay ()
{
	APlayerController* PlayerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);

	PlayerController->SetViewTargetWithBlend(MainCameraActor, 2.0f, EViewTargetBlendFunction::VTBlend_Linear, 2.0f, false);
}
//------------------------------------------------------------------------------------------------------------
void UW_MainMenu::OnClickedButtonExit ()
{
	APlayerController* PlayerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);

	UKismetSystemLibrary::QuitGame(GetWorld(), PlayerController, EQuitPreference::Quit, false);
}
//------------------------------------------------------------------------------------------------------------
void UW_MainMenu::OnHoveredButtonExit ()
{
	APlayerController* PlayerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);

	PlayerController->SetViewTargetWithBlend(ExitCameraActor, 2.0f, EViewTargetBlendFunction::VTBlend_Linear, 2.0f, false);
}
//------------------------------------------------------------------------------------------------------------
void UW_MainMenu::OnUnhoveredButtonExit ()
{
	APlayerController* PlayerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);

	PlayerController->SetViewTargetWithBlend(MainCameraActor, 2.0f, EViewTargetBlendFunction::VTBlend_Linear, 2.0f, false);
}
//------------------------------------------------------------------------------------------------------------
