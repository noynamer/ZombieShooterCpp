#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "W_DeathWidget.generated.h"

class UUniformGridPanel;
class UVerticalBox;
class UTextBlock;
class UButton;

UCLASS()
class ZOMBIESHOOTER_API UW_DeathWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:

	virtual void NativeConstruct() override;

	UFUNCTION()
	void OnClickedButtonAgain();

	UFUNCTION()
	void OnClickedButtonExit();

	UPROPERTY (BlueprintReadOnly, Category = "UI", meta = (BindWidget))
	UUniformGridPanel* GridPanel;

	UPROPERTY (BlueprintReadOnly, Category = "UI", meta = (BindWidget))
	UVerticalBox* VerticalBox;

	UPROPERTY (BlueprintReadOnly, Category = "UI", meta = (BindWidget))
	UTextBlock* TextBlockDeath;

	UPROPERTY (BlueprintReadOnly, Category = "UI", meta = (BindWidget))
	UButton* ButtonAgain;

	UPROPERTY (BlueprintReadOnly, Category = "UI", meta = (BindWidget))
	UTextBlock* TextBlockAgain;

	UPROPERTY (BlueprintReadOnly, Category = "UI", meta = (BindWidget))
	UButton* ButtonExit;

	UPROPERTY (BlueprintReadOnly, Category = "UI", meta = (BindWidget))
	UTextBlock* TextBlockExit;

	FName DemoDayName = "Demo_Day";
};
