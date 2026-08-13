#include "MainWidget.h"
#include "../SwatCharacter.h"

#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetStringLibrary.h"
#include "Components/TextBlock.h"
#include "Components/ProgressBar.h"

//------------------------------------------------------------------------------------------------------------
void UMainWidget::NativeTick (const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	OnCountDown();

	if (TimerLevel <= 0)
	{
		RemoveFromParent();

		if (WinWidgetClass)
		{
			WinWidget = CreateWidget<UUserWidget>(GetWorld(), WinWidgetClass);

			WinWidget->AddToViewport();
		}
	}
}
//------------------------------------------------------------------------------------------------------------
void UMainWidget::NativeConstruct ()
{
	Super::NativeConstruct();
}
//------------------------------------------------------------------------------------------------------------
void UMainWidget::OnCountDown ()
{
	TimerLevel = FMath::Clamp(TimerLevel - UGameplayStatics::GetWorldDeltaSeconds(GetWorld()), 0.0f, TimerLevel);

	FString TimerLevelString = UKismetStringLibrary::TimeSecondsToString(TimerLevel);
	FText TimerLevelText = FText::FromString(TimerLevelString);

	TimerCounterText->SetText(TimerLevelText);
}
//------------------------------------------------------------------------------------------------------------
