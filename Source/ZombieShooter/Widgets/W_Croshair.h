#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "W_Croshair.generated.h"

class UImage;

UCLASS()
class ZOMBIESHOOTER_API UW_Croshair : public UUserWidget
{
	GENERATED_BODY()
	
public:

	UPROPERTY (BlueprintReadOnly, meta = (BindWidget))
	UImage* MainCrosshairImage;

};
