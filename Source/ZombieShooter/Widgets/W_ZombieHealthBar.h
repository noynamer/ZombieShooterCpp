#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "W_ZombieHealthBar.generated.h"

class UProgressBar;

UCLASS()
class ZOMBIESHOOTER_API UW_ZombieHealthBar : public UUserWidget
{
	GENERATED_BODY()
	
public:

	virtual void NativeConstruct() override;
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

	UPROPERTY (BlueprintReadOnly, Category = "UI", meta = (BindWidget))
	UProgressBar* HealthZombieProgressBar;

};
