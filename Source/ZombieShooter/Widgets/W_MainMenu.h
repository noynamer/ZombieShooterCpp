#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "W_MainMenu.generated.h"

class UTextBlock;
class UImage;
class UButton;

UCLASS()
class ZOMBIESHOOTER_API UW_MainMenu : public UUserWidget
{
	GENERATED_BODY()
	
public:

	UPROPERTY (BlueprintReadOnly, Category = "UI", meta = (BindWidget))
	UTextBlock* ZombieShooterTextBlock;

	UPROPERTY (BlueprintReadOnly, Category = "UI", meta = (BindWidget))
	UImage* ZombieFirstImage;

	UPROPERTY (BlueprintReadOnly, Category = "UI", meta = (BindWidget))
	UImage* ZombieSecondImage;

	UPROPERTY (BlueprintReadOnly, Category = "UI", meta = (BindWidget))
	UButton* BeginPlayButton;

	UPROPERTY (BlueprintReadOnly, Category = "UI", meta = (BindWidget))
	UButton* ExitButton;

	UPROPERTY (BlueprintReadOnly, Category = "UI", meta = (BindWidget))
	UTextBlock* BeginPlayTextBlock;

	UPROPERTY (BlueprintReadOnly, Category = "UI", meta = (BindWidget))
	UTextBlock* ExitTextBlock;

};
