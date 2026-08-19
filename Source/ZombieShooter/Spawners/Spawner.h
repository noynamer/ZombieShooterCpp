#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Spawner.generated.h"

class UBoxComponent;

UCLASS()
class ZOMBIESHOOTER_API ASpawner : public AActor
{
	GENERATED_BODY()
	
public:	

	ASpawner();
	virtual void Tick(float DeltaTime) override;
	virtual void BeginPlay() override;

	UFUNCTION(BlueprintCallable)
	void SpawnerAmmo();

	UFUNCTION(BlueprintCallable)
	void SpawnActor();

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float TimeBetweenSpawn = 30.0f;

	UPROPERTY (VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UBoxComponent* Box;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSubclassOf<AActor> SpawnClass;

	UPROPERTY(BlueprintReadWrite)
	FTimerHandle TimeSpawn;
};
