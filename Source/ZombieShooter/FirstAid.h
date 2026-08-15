#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interfaces/I_FirstAid.h"
#include "FirstAid.generated.h"

class UBoxComponent;
class USpotLightComponent;
class URotatingMovementComponent;

UCLASS()
class ZOMBIESHOOTER_API AFirstAid : public AActor, public II_FirstAid
{
	GENERATED_BODY()
	
public:	

	AFirstAid();
	virtual void Tick(float DeltaTime) override;
	void NotifyActorBeginOverlap(AActor* OtherActor) override;
	void AddHealth_Implementation() override;

	UPROPERTY (VisibleAnywhere, Category = "Components")
	USceneComponent* DefaultSceneRootT;

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

};
