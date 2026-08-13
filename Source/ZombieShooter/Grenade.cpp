#include "Grenade.h"

#include "GameFramework/ProjectileMovementComponent.h"
#include "Kismet/GameplayStatics.h"

//------------------------------------------------------------------------------------------------------------
AGrenade::AGrenade()
{
	PrimaryActorTick.bCanEverTick = true;

	StaticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	StaticMeshComponent->SetCollisionProfileName(TEXT("Projectile"));

	ProjectileMovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("Projectile"));
	ProjectileMovementComponent->InitialSpeed = 500.0f;
	ProjectileMovementComponent->MaxSpeed = 700.0f;
}
//------------------------------------------------------------------------------------------------------------
void AGrenade::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}
//------------------------------------------------------------------------------------------------------------
void AGrenade::Explode ()
{
	UWorld* World = GetWorld();
	if (!World) return;

	FVector ExplosionLocation = GetActorLocation();

	UGameplayStatics::ApplyRadialDamage(
		World,
		BaseDamage,
		ExplosionLocation,
		DamageRadius,
		UDamageType::StaticClass(),
		TArray<AActor*>(),
		this,
		GetInstigatorController(),
		false,
		ECC_Visibility
	);

	if (ExplosionParticle)
	{
		UGameplayStatics::SpawnEmitterAtLocation(
			World,
			ExplosionParticle,
			ExplosionLocation,
			FRotator::ZeroRotator,
			FVector(1.0f, 1.0f, 1.0f),
			true,
			EPSCPoolMethod::None,
			true
		);
	}

	if (ExplosionSound)
	{
		UGameplayStatics::SpawnSoundAtLocation(
			World,
			ExplosionSound,
			ExplosionLocation,
			FRotator::ZeroRotator,
			0.4f,
			1.0f,
			0.0f,
			nullptr,
			nullptr,
			true
		);
	}

	Destroy();
}
//------------------------------------------------------------------------------------------------------------
void AGrenade::BeginPlay()
{
	Super::BeginPlay();
	
	GetWorld()->GetTimerManager().SetTimer(
		ExplosionTimerHandle,
		this,
		&AGrenade::Explode,
		1.0f,
		false
	);
}
//------------------------------------------------------------------------------------------------------------
