#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SpawnerEnemy.generated.h"

class UBoxComponent;

UCLASS()
class ZOMBIESHOOTER_API ASpawnerEnemy : public AActor
{
	GENERATED_BODY()
	
public:	

	ASpawnerEnemy();
	virtual void Tick(float DeltaTime) override;
	virtual void BeginPlay() override;

	UFUNCTION(BlueprintCallable)
	void OnSpawner();

	UFUNCTION(BlueprintCallable)
	void OnMakeCountEnemies();

	UPROPERTY (VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	USceneComponent* DefaultSceneRoot;

	UPROPERTY (VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UBoxComponent* BoxSpawner;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSubclassOf<AActor> SpawnClass;

	UPROPERTY(BlueprintReadWrite)
	int EnemiesCount = 0;

private:

	FTimerHandle SpawnTimerHandle;
};
