#include "Spawner.h"

#include "Components/BoxComponent.h"

//------------------------------------------------------------------------------------------------------------
ASpawner::ASpawner()
{
	PrimaryActorTick.bCanEverTick = true;

	Box = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxCollision"));
	RootComponent = Box;
}
//------------------------------------------------------------------------------------------------------------
void ASpawner::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}
//------------------------------------------------------------------------------------------------------------
void ASpawner::BeginPlay()
{
	Super::BeginPlay();
	
	SpawnerAmmo();
}
//------------------------------------------------------------------------------------------------------------
void ASpawner::SpawnerAmmo()
{
	if (GetWorld() && SpawnClass)
	{
		GetWorld()->GetTimerManager().SetTimer(
			TimeSpawn,
			this,
			&ASpawner::SpawnActor,
			TimeBetweenSpawn,
			true
		);
	}
}
//------------------------------------------------------------------------------------------------------------
void ASpawner::SpawnActor ()
{
	FVector Center = Box->GetComponentLocation();
	FVector HalfSize = Box->GetScaledBoxExtent();
	FVector SpawnPoint = FMath::RandPointInBox(FBox::BuildAABB(Center, HalfSize));
	FVector Scale = FVector(2.0f, 2.0f, 2.0f);

	FTransform Transform;
	Transform.SetLocation(SpawnPoint);
	Transform.SetRotation(FRotator::ZeroRotator.Quaternion());
	Transform.SetScale3D(Scale);

	FActorSpawnParameters Params;
	Params.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;

	AActor* NewActor = GetWorld()->SpawnActor<AActor>(
		SpawnClass,
		Transform,
		Params
	);
}
//------------------------------------------------------------------------------------------------------------
