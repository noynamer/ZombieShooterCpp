#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"

#include "Interfaces/I_Ammo.h"
#include "Interfaces/I_FirstAid.h"

#include "Components/TimelineComponent.h"
#include "Curves/CurveFloat.h"
#include "InputActionValue.h"

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
class UInputAction;

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

//-----------------------------Damage--------------------------------------------------------------------------
	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, 
		class AController* EventInstigator, AActor* DamageCauser) override;
	void OnTakeDamage(float Damage);
	void OnHitEffect();

	void SpawnGrenade();
	void OnOnceShoot();
	void OnStopShoot();

//-----------------------------Input--------------------------------------------------------------------------
	void Move(const FInputActionValue& Value);
	void Look(const FInputActionValue& Value);
	void SprintTriggered();
	void SprintCanceled();
	void SprintCompleted();
	void IaAimingStarted();
	void IaAimingCanceledAndCompleted();

	void DelayHitEffect();
	void WeaponAmmoCounter();
	void SetPlayerController();
	void SpawnWeapon();
	void SpawnWidget();
	void OnCreateMainWidget();
	void OnCreateCrosshairWidget();
	void SoundEmptyWeaponAndFiringMontage();
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
	bool bPlayedEmptySound = false;

	UPROPERTY (EditDefaultsOnly, Category = "Sound")
	USoundBase* SoundEmptyWeapon;

	UPROPERTY (EditDefaultsOnly, Category = "Sound")
	USoundBase* SoundFireWeapon;

	UPROPERTY (EditDefaultsOnly, Category = "Sound")
	USoundBase* SoundHitMaterial;

	UPROPERTY (EditDefaultsOnly, Category = "Sound")
	USoundBase* SoundHitZombie;

	UPROPERTY (EditDefaultsOnly, Category = "Attenuation")
	USoundAttenuation* SA_HitSound;

	UPROPERTY (EditDefaultsOnly, Category = "Animation")
	UAnimMontage* FiringMontage;

	UPROPERTY (EditDefaultsOnly, Category = "ParticleSystem")
	UParticleSystem* P_ImpactBlood;

	UParticleSystem* SelectedEmitter = nullptr;

	UPROPERTY (EditDefaultsOnly, Category = "ParticleSystem")
	UParticleSystem* P_ImpactPlaster;

	UPROPERTY (EditDefaultsOnly, Category = "ParticleSystem")
	UParticleSystem* P_ImpactBrick;

	UPROPERTY (EditDefaultsOnly, Category = "ParticleSystem")
	UParticleSystem* P_ImpactConcrete;

	UPROPERTY (EditDefaultsOnly, Category = "PostProcess")
	FPostProcessSettings HitPostProcessSettings;

	UPROPERTY(EditDefaultsOnly, Category = "SpawnClass")
	TSubclassOf<AActor> WeaponClass;

	UPROPERTY(EditDefaultsOnly, Category = "SpawnClass")
	TSubclassOf<AActor> GrenadeClass;

	UPROPERTY(EditDefaultsOnly, Category = "SpawnClass")
	TSubclassOf<UUserWidget> CrosshairWidgetClass;

	UPROPERTY(EditDefaultsOnly, Category = "SpawnClass")
	TSubclassOf<UMainWidget> MainWidgetClass;

	UPROPERTY(EditDefaultsOnly, Category = "SpawnClass")
	TSubclassOf<UUserWidget> DeathWidgetClass;

	UPROPERTY(EditDefaultsOnly, Category = "Input")
	TObjectPtr<UInputMappingContext> IMC_Default;

	UPROPERTY(EditDefaultsOnly, Category = "Input")
	TObjectPtr<UInputAction> IA_Move;

	UPROPERTY(EditDefaultsOnly, Category = "Input")
	TObjectPtr<UInputAction> IA_Look;

	UPROPERTY(EditDefaultsOnly, Category = "Input")
	TObjectPtr<UInputAction> IA_Jump;

	UPROPERTY(EditDefaultsOnly, Category = "Input")
	TObjectPtr<UInputAction> IA_Sprint;

	UPROPERTY(EditDefaultsOnly, Category = "Input")
	TObjectPtr<UInputAction> IA_Aiming;

	UPROPERTY(EditDefaultsOnly, Category = "Input")
	TObjectPtr<UInputAction> IA_Shoot;

	UPROPERTY(EditDefaultsOnly, Category = "Input")
	TObjectPtr<UInputAction> IA_ThrowGrenade;

};
