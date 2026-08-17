#include "MainMenuGame.h"

#include "Blueprint/UserWidget.h"
#include "Kismet/GameplayStatics.h"

//------------------------------------------------------------------------------------------------------------
void AMainMenuGame::BeginPlay ()
{
	APlayerController* PlayerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);

	if (MainMenuWidgetClass && MainCamera)
	{
		PlayerController->SetViewTargetWithBlend (MainCamera, 0.0f, EViewTargetBlendFunction::VTBlend_Linear, 0.0f, false);
	}

	UUserWidget* MainMenuWidget = CreateWidget<UUserWidget>(PlayerController, MainMenuWidgetClass);

	MainMenuWidget->AddToViewport();
}
//------------------------------------------------------------------------------------------------------------

