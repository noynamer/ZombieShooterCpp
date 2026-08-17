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

	virtual void NativeConstruct() override;

	UFUNCTION()
	void OnClickedButtonBeginPlay();

	UFUNCTION()
	void OnHoveredButtonBeginPlay();

	UFUNCTION()
	void OnUnhoveredButtonBeginPlay();

	UFUNCTION()
	void OnClickedButtonExit();

	UFUNCTION()
	void OnHoveredButtonExit();

	UFUNCTION()
	void OnUnhoveredButtonExit();

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

	UPROPERTY(EditDefaultsOnly, Category = "SpawnClass")
	TSubclassOf<AActor> StartCameraClass;

	UPROPERTY(BlueprintReadWrite)
	AActor* StartCameraActor;

	UPROPERTY(EditDefaultsOnly, Category = "SpawnClass")
	TSubclassOf<AActor> MainCameraClass;

	UPROPERTY(BlueprintReadWrite)
	AActor* MainCameraActor;

	UPROPERTY(EditDefaultsOnly, Category = "SpawnClass")
	TSubclassOf<AActor> ExitCameraClass;

	UPROPERTY(BlueprintReadWrite)
	AActor* ExitCameraActor;

	FName DemoDayName = "Demo_Day";

};
