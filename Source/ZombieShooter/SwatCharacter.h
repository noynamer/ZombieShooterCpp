#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"

#include "Interfaces/I_Ammo.h"
#include "Interfaces/I_FirstAid.h"

#include "Components/TimelineComponent.h"
#include "Curves/CurveFloat.h"

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
class UInputMappingContext;

UCLASS()
class ZOMBIESHOOTER_API ASwatCharacter : public ACharacter, public II_Ammo, public II_FirstAid
{
	GENERATED_BODY()

public:

	ASwatCharacter();
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	virtual void PostInitializeComponents() override;

	FVector LeftHandSocketPoseCharacter;

	float HealthCharacter = 1.0f;

	bool bIsAimingMY = false;

	int AmmoCharacter = 60;
	int AmmoMin = 0;
	int AmmoMax = 60;

private:

	void DelayHitEffect();
	void WeaponAmmoCounter();
	void SetPlayerController();
	void SpawnWeapon();
	void SpawnWidget();
	void OnCreateMainWidget();
	void OnCreateCrosshairWidget();
	void SoundEmptyWeaponAndFiringMontage();
	void OnHitEffect();
	void OnCreateDeathWidget();
	void OnLineTrace();
	void TLAimUpdate(float Zoom);
	void TLAimInitialize();

	UMainWidget* MainWidgetREF;
	UW_Croshair* W_CroshairREF;
	AWeapon* WeaponREF;
	UUserWidget* DeathWidget;

	USpringArmComponent* SpringArmComponent;
	UCameraComponent* CameraComponent;

	FName WeaponSocket = "WeaponSocket";
	FName LeftHandSocketPos = "LeftHandSocketPos";

	FTimerHandle DelayPostProcessTimer;
	FTimerHandle DelayTakeDamageTimer;
	FTimerHandle DelayOnShootTimer;

	UCurveFloat* AimCurve;

	FTimeline TLAim;

	int GrenadeAmount = 3;
	int GrenadeMin = 0;
	int GrenadeMax = 3;

	bool bCanShootMY = false;

	UFUNCTION(BlueprintCallable)
	void SpawnGrenade();

	UFUNCTION(BlueprintCallable)
	void OnOnceShoot();

	UFUNCTION(BlueprintCallable)
	void OnStopShoot();

	UFUNCTION(BlueprintCallable)
	void OnTakeDamage(float Damage);

	UFUNCTION(BlueprintCallable)
	void IaAimingStarted();

	UFUNCTION(BlueprintCallable)
	void IaAimingCanceledAndCompleted();

	UPROPERTY (EditAnywhere, Category = "Sound")
	USoundBase* SoundEmptyWeapon;

	UPROPERTY (EditAnywhere, Category = "Sound")
	USoundBase* SoundFireWeapon;

	UPROPERTY (EditAnywhere, Category = "Sound")
	USoundBase* SoundHitMaterial;

	UPROPERTY (EditAnywhere, Category = "Sound")
	USoundBase* SoundHitZombie;

	UPROPERTY (EditAnywhere, Category = "Attenuation")
	USoundAttenuation* SA_HitSound;

	UPROPERTY (EditAnywhere, Category = "Animation")
	UAnimMontage* FiringMontage;

	UPROPERTY (EditAnywhere, Category = "ParticleSystem")
	UParticleSystem* P_ImpactBlood;

	UParticleSystem* SelectedEmitter = nullptr;

	UPROPERTY (EditAnywhere, Category = "ParticleSystem")
	UParticleSystem* P_ImpactPlaster;

	UPROPERTY (EditAnywhere, Category = "ParticleSystem")
	UParticleSystem* P_ImpactBrick;

	UPROPERTY (EditAnywhere, Category = "ParticleSystem")
	UParticleSystem* P_ImpactConcrete;

	UPROPERTY (EditAnywhere, Category = "PostProcess")
	FPostProcessSettings HitPostProcessSettings;

	UPROPERTY(EditAnywhere, Category = "SpawnClass")
	TSubclassOf<AActor> WeaponClass;

	UPROPERTY(EditAnywhere, Category = "SpawnClass")
	TSubclassOf<AActor> GrenadeClass;

	UPROPERTY(EditAnywhere, Category = "SpawnClass")
	TSubclassOf<UUserWidget> CrosshairWidgetClass;

	UPROPERTY(EditAnywhere, Category = "SpawnClass")
	TSubclassOf<UMainWidget> MainWidgetClass;

	UPROPERTY(EditAnywhere, Category = "SpawnClass")
	TSubclassOf<UUserWidget> DeathWidgetClass;

	UPROPERTY(EditAnywhere, Category = "Input")
	UInputMappingContext* IMC_Default;

};
