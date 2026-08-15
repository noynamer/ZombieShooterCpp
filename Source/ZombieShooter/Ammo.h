#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interfaces/I_Ammo.h"
#include "Ammo.generated.h"

//class ASwatCharacter;

class UBoxComponent;
class USpotLightComponent;
class URotatingMovementComponent;

UCLASS()
class ZOMBIESHOOTER_API AAmmo : public AActor, public II_Ammo
{
	GENERATED_BODY()
	
public:	

	AAmmo();
	virtual void Tick(float DeltaTime) override;
	void NotifyActorBeginOverlap(AActor* OtherActor) override;
	void AddAmmo_Implementation() override;

	UPROPERTY (VisibleAnywhere, Category = "Components")
	USceneComponent* DefaultSceneRoot;

	UPROPERTY (VisibleAnywhere, Category = "Components")
	UBoxComponent* Box;

	UPROPERTY (VisibleAnywhere, Category = "Components")
	USpotLightComponent* SpotLight;

	UPROPERTY (EditDefaultsOnly, Category = "Components")
	UStaticMeshComponent* StaticMesh;

	UPROPERTY (VisibleAnywhere, Category = "Components")
	URotatingMovementComponent* RotatingMovement;

	UPROPERTY (EditDefaultsOnly, Category = "Effects")
	USoundBase* PickupSound;

protected:

	virtual void BeginPlay() override;

private:

};
