#include "Weapon.h"

#include "TimerManager.h"
#include "Components/ArrowComponent.h"
#include "NiagaraSystem.h"
#include "NiagaraComponent.h"
#include "NiagaraFunctionLibrary.h"
#include "Components/PointLightComponent.h"

//------------------------------------------------------------------------------------------------------------
AWeapon::AWeapon()
{
	PrimaryActorTick.bCanEverTick = true;

	DefaultSceneRoot = CreateDefaultSubobject<USceneComponent>(TEXT("DefaultSceneRoot"));
	RootComponent = DefaultSceneRoot;

	ArrowComponent = CreateDefaultSubobject<UArrowComponent>(TEXT("ArrowComponent"));
	ArrowComponent->SetupAttachment(RootComponent);

	WeaponMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("SK_Weapon"));
	WeaponMesh->SetupAttachment(RootComponent);

	PointLightComponent = CreateDefaultSubobject<UPointLightComponent>(TEXT("PointLightComponent"));
	PointLightComponent->SetupAttachment(RootComponent);
	PointLightComponent->SetRelativeLocation(FVector(-0.00591f, 80.980611f, 8.262983));
	PointLightComponent->AttenuationRadius = 50.0f;
	PointLightComponent->LightColor = FColor::Yellow;
	PointLightComponent->Intensity = 5000.0f;

	MuzzleFlashEffect = nullptr;
}
//------------------------------------------------------------------------------------------------------------
void AWeapon::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}
//------------------------------------------------------------------------------------------------------------
void AWeapon::DestroyMuzzleFlash ()
{
	if (SpawnedMuzzleFlashComponent && IsValid(SpawnedMuzzleFlashComponent))
	{
		SpawnedMuzzleFlashComponent->DestroyComponent();
		SpawnedMuzzleFlashComponent = nullptr;
	}
}
//------------------------------------------------------------------------------------------------------------
void AWeapon::SpawnLightWithDelay ()
{
	FRotator Rotation = ArrowComponent->GetComponentRotation ();
	Rotation.Yaw += 90.0f;

	if (MuzzleFlashEffect)
	{
		SpawnedMuzzleFlashComponent = UNiagaraFunctionLibrary::SpawnSystemAttached (
			MuzzleFlashEffect,
			ArrowComponent,
			NAME_None,
			FVector::ZeroVector,
			Rotation,
			EAttachLocation::KeepRelativeOffset,
			true,
			true
		);

		GetWorld()->GetTimerManager().SetTimer(
			DestroyTimerHandle,
			this,
			&AWeapon::DestroyMuzzleFlash,
			0.03f,
			false
		);
	}

	if (PointLightComponent)
	{
		PointLightComponent->SetHiddenInGame(false, true);
	}

	GetWorld()->GetTimerManager().SetTimer(
		LightTimerHandle,
		this,
		&AWeapon::TurnOffLight,
		0.1f,
		false
	);
}
//------------------------------------------------------------------------------------------------------------
void AWeapon::TurnOffLight ()
{
	if (PointLightComponent && IsValid(PointLightComponent))
	{
		PointLightComponent->SetHiddenInGame(true, true);
	}
}
//------------------------------------------------------------------------------------------------------------
void AWeapon::BeginPlay()
{
	Super::BeginPlay();
}
//------------------------------------------------------------------------------------------------------------
