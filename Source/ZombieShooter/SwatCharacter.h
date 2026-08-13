#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Interfaces/I_Ammo.h"
#include "Interfaces/I_FirstAid.h"
#include "SwatCharacter.generated.h"

class AWeapon;
class UMainWidget;
class UW_Croshair;

class UArrowComponent;
class UCapsuleComponent;
class USpringArmComponent;
class UCameraComponent;
class USoundBase;
class USoundAttenuation;

UCLASS()
class ZOMBIESHOOTER_API ASwatCharacter : public ACharacter, public II_Ammo, public II_FirstAid
{
	GENERATED_BODY()

public:

	ASwatCharacter();
	//virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	void DelayHitEffect();

	UFUNCTION(BlueprintCallable)
	void WeaponAmmoCounter();

	UFUNCTION(BlueprintCallable)
	void SpawnWeapon();

	UFUNCTION(BlueprintCallable)
	void SpawnWidget();

	UFUNCTION(BlueprintCallable)
	void SpawnMainWidget();

	UFUNCTION(BlueprintCallable)
	void SpawnCroshairWidget();

	UFUNCTION(BlueprintCallable)
	void SpawnGrenade();

	UFUNCTION(BlueprintCallable)
	void SoundEmptyWeaponAndFiringMontage();

	UFUNCTION(BlueprintCallable)
	void OnOnceShoot();

	UFUNCTION(BlueprintCallable)
	void OnStopShoot();

	UFUNCTION(BlueprintCallable)
	void OnHitEffect();

	UFUNCTION(BlueprintCallable)
	void OnTakeDamage(float Damage);

	UFUNCTION(BlueprintCallable)
	void OnCreateDeathWidget();

	UFUNCTION(BlueprintCallable)
	void OnLineTrace();

	FName WeaponSocket = "WeaponSocket";

	UPROPERTY (EditAnywhere, BlueprintReadOnly, Category = "Components")
	USpringArmComponent* SpringArmComponent;

	UPROPERTY (EditAnywhere, BlueprintReadOnly, Category = "Components")
	UCameraComponent* CameraComponent;

	UPROPERTY (EditAnywhere, BlueprintReadOnly, Category = "Sound")
	USoundBase* SoundEmptyWeapon;

	UPROPERTY (EditAnywhere, BlueprintReadOnly, Category = "Sound")
	USoundBase* SoundFireWeapon;

	UPROPERTY (EditAnywhere, BlueprintReadOnly, Category = "Sound")
	USoundBase* SoundHitMaterial;

	UPROPERTY (EditAnywhere, BlueprintReadOnly, Category = "Sound")
	USoundBase* SoundHitZombie;

	UPROPERTY (EditAnywhere, BlueprintReadOnly, Category = "Attenuation")
	USoundAttenuation* SA_HitSound;

	UPROPERTY (EditAnywhere, BlueprintReadOnly, Category = "Animation")
	UAnimMontage* FiringMontage;

	UPROPERTY (EditAnywhere, BlueprintReadWrite, Category = "ParticleSystem")
	UParticleSystem* P_ImpactBlood;

	UPROPERTY (VisibleAnywhere, BlueprintReadWrite, Category = "ParticleSystem")
	UParticleSystem* SelectedEmitter = nullptr;

	UPROPERTY (EditAnywhere, BlueprintReadWrite, Category = "ParticleSystem")
	UParticleSystem* P_ImpactPlaster;

	UPROPERTY (EditAnywhere, BlueprintReadWrite, Category = "ParticleSystem")
	UParticleSystem* P_ImpactBrick;

	UPROPERTY (EditAnywhere, BlueprintReadWrite, Category = "ParticleSystem")
	UParticleSystem* P_ImpactConcrete;

	UPROPERTY (EditAnywhere, BlueprintReadWrite, Category = "PostProcess")
	FPostProcessSettings HitPostProcessSettings;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSubclassOf<AActor> WeaponClass;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSubclassOf<AActor> GrenadeClass;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSubclassOf<UUserWidget> CrosshairWidgetClass;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSubclassOf<UUserWidget> MainWidgetClass;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSubclassOf<UUserWidget> DeathWidgetClass;

	UPROPERTY(BlueprintReadWrite)
	UUserWidget* CrosshairWidget;

	UPROPERTY(BlueprintReadWrite)
	UUserWidget* MainWidget;

	UPROPERTY(BlueprintReadWrite)
	UUserWidget* DeathWidget;

	UPROPERTY (BlueprintReadWrite)
	float HealthCharacter = 1.0f;

	UPROPERTY (BlueprintReadWrite)
	FVector LeftHandSocketPoseCharacter;

	UPROPERTY (BlueprintReadWrite)
	FName LeftHandSocketPos = "LeftHandSocketPos";

	UPROPERTY (EditAnywhere, BlueprintReadWrite)
	int GrenadeAmount = 3;

	UPROPERTY (BlueprintReadWrite)
	int GrenadeMin = 0;

	UPROPERTY (BlueprintReadWrite)
	int GrenadeMax = 3;

	UPROPERTY (EditAnywhere, BlueprintReadWrite)
	int AmmoCharacter = 60;

	UPROPERTY (BlueprintReadWrite)
	int AmmoMin = 0;

	UPROPERTY (BlueprintReadWrite)
	int AmmoMax = 60;
	
	UPROPERTY(BlueprintReadWrite)
	bool bCanShootMY = false;

	UPROPERTY(BlueprintReadWrite)
	bool bIsAimingMY = false;

	UPROPERTY(BlueprintReadWrite)
	UMainWidget* MainWidgetREF;

	UPROPERTY(BlueprintReadWrite)
	UW_Croshair* W_CroshairREF;

	UPROPERTY(BlueprintReadWrite)
	AWeapon* WeaponREF;

private:	

	FTimerHandle DelayPostProcessTimer;
	FTimerHandle DelayTakeDamageTimer;
	FTimerHandle DelayOnShootTimer;

};
