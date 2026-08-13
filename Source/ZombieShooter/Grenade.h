#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Grenade.generated.h"

class UProjectileMovementComponent;

UCLASS()
class ZOMBIESHOOTER_API AGrenade : public AActor
{
	GENERATED_BODY()
	
public:	

	AGrenade();
	virtual void Tick(float DeltaTime) override;

	UPROPERTY (VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UStaticMeshComponent* StaticMeshComponent;

	UPROPERTY (VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UProjectileMovementComponent* ProjectileMovementComponent;

	UPROPERTY (VisibleAnywhere, BlueprintReadWrite, Category = "Explosion")
	float BaseDamage = 3.0f;

	UPROPERTY (VisibleAnywhere, BlueprintReadWrite, Category = "Explosion")
	float DamageRadius = 500.0f;

	UPROPERTY (EditAnywhere, BlueprintReadWrite, Category = "Effects")
	UParticleSystem* ExplosionParticle;

	UPROPERTY (EditAnywhere, BlueprintReadWrite, Category = "Effects")
	USoundBase* ExplosionSound;

	UFUNCTION()
	void Explode();

protected:

	virtual void BeginPlay() override;

private:

	FTimerHandle ExplosionTimerHandle;

};
