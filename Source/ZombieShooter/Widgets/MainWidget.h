#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MainWidget.generated.h"

class ASwatCharacter;

class UTextBlock;
class UProgressBar;
class UImage;

UCLASS()
class ZOMBIESHOOTER_API UMainWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:

	void NativeTick (const FGeometry& MyGeometry, float InDeltaTime) override;
	virtual void NativeConstruct() override;

	UFUNCTION(BlueprintCallable)
	void OnCountDown();

	UPROPERTY (BlueprintReadOnly, Category = "UI", meta = (BindWidget))
	UTextBlock* AmmoTextBlock;

	UPROPERTY (BlueprintReadOnly, Category = "UI", meta = (BindWidget))
	UTextBlock* GrenadeCounterText;

	UPROPERTY (BlueprintReadOnly, Category = "UI", meta = (BindWidget))
	UTextBlock* TimerCounterText;

	UPROPERTY (BlueprintReadOnly, Category = "UI", meta = (BindWidget))
	UImage* GrenadeImage;

	UPROPERTY (BlueprintReadOnly, Category = "UI", meta = (BindWidget))
	UImage* HealthImage;

	UPROPERTY (BlueprintReadOnly, Category = "UI", meta = (BindWidget))
	UImage* WeaponImage;

	UPROPERTY (BlueprintReadOnly, Category = "UI", meta = (BindWidget))
	UProgressBar* HealthCharacterProgressBar;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "UI")
	TSubclassOf<UUserWidget> WinWidgetClass;

	UPROPERTY(BlueprintReadOnly)
	UUserWidget* WinWidget;

	UPROPERTY(BlueprintReadWrite)
	float TimerLevel = 50.0f;

	UPROPERTY(BlueprintReadOnly)
	ASwatCharacter* SwatCharacterREF;
};
