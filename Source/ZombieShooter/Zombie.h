#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "NavigationSystem.h"
#include "Navigation/PathFollowingComponent.h"
#include "Zombie.generated.h"

DECLARE_MULTICAST_DELEGATE_OneParam(FOnZombieHealthChanged, float);

class UW_ZombieHealthBar;

class UWidgetComponent;
class USphereComponent;

UCLASS()
class ZOMBIESHOOTER_API AZombie : public ACharacter
{
	GENERATED_BODY()

public:

	AZombie();
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	virtual void BeginPlay() override;
	
	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, 
		class AController* EventInstigator, AActor* DamageCauser) override;
	virtual float TakeRadialDamage(float DamageAmount, struct FRadialDamageEvent const& RadialDamageEvent, 
		class AController* EventInstigator, AActor* DamageCauser);
	void OnTakeDamage(float Damage);
	void OnGetHit();
	void OnMoveToCharacter();
	void OnMoveToCharacterUpdate();
	void ResetGetCharacter();
	FVector OnFuncMoveToCharacter();

	UFUNCTION()
	void OnMoveFinished(FAIRequestID RequestID, EPathFollowingResult::Type Result);
	FOnZombieHealthChanged OnHealthChanged;

	UPROPERTY (EditAnywhere, BlueprintReadOnly, Category = "Components")
	USphereComponent* HitSphere;

	UPROPERTY (VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UWidgetComponent* WidgetZombieHealth;

	UPROPERTY (EditAnywhere, BlueprintReadOnly)
	UAnimMontage* ZombieAttackMontage;

	UPROPERTY (BlueprintReadWrite)
	float HealthZombie = 1.0f;

	UPROPERTY (BlueprintReadWrite)
	bool bIsGetCharacterMY = false;

private:

	float SphereRadius = 32.0f;
	float BaseDamage = 0.2f;
	FTimerHandle MoveToCharacterTimer;
	FTimerHandle ResetTimer;
	FTimerHandle HitTimer;

	UW_ZombieHealthBar* W_ZombieHealthBarREF;
};
