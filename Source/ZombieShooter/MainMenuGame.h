#pragma once

#include "CoreMinimal.h"
#include "Engine/LevelScriptActor.h"
#include "MainMenuGame.generated.h"

class UUserWidget;

UCLASS()
class ZOMBIESHOOTER_API AMainMenuGame : public ALevelScriptActor
{
	GENERATED_BODY()
	
public:

	virtual void BeginPlay() override;

	UPROPERTY(EditDefaultsOnly)
	AActor* MainCamera;

	UPROPERTY (EditDefaultsOnly, Category = "Spawn Class")
	TSubclassOf<UUserWidget> MainMenuWidgetClass;
};
