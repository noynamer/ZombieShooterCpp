#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Weapon.generated.h"

class UArrowComponent;
class UNiagaraSystem;
class UNiagaraComponent;
class UPointLightComponent;

UCLASS()
class ZOMBIESHOOTER_API AWeapon : public AActor
{
	GENERATED_BODY()
	
public:	

	AWeapon();
	virtual void Tick(float DeltaTime) override;

	void DestroyMuzzleFlash();
	void TurnOffLight();

	UPROPERTY (EditDefaultsOnly, BlueprintReadWrite, Category = "Effects")
	UNiagaraSystem* MuzzleFlashEffect;

	UPROPERTY()
	UNiagaraComponent* SpawnedMuzzleFlashComponent = nullptr;

	UPROPERTY (EditAnywhere, BlueprintReadWrite, Category = "Components")
	UPointLightComponent* PointLightComponent;

	UPROPERTY (EditAnywhere, BlueprintReadWrite, Category = "Components")
	UArrowComponent* ArrowComponent;

	UPROPERTY (EditAnywhere, BlueprintReadWrite, Category = "Components")
	USkeletalMeshComponent* WeaponMesh;

	UPROPERTY (VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	USceneComponent* DefaultSceneRoot;

	UFUNCTION(BlueprintCallable)
	void SpawnLightWithDelay();

protected:

	virtual void BeginPlay() override;

private:

	FTimerHandle DelayTimerHandle;
	FTimerHandle DestroyTimerHandle;
	FTimerHandle LightTimerHandle;

};
