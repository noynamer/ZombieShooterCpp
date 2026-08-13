#include "SpawnerEnemy.h"

#include "Components/BoxComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"

//------------------------------------------------------------------------------------------------------------
ASpawnerEnemy::ASpawnerEnemy()
{
	PrimaryActorTick.bCanEverTick = true;

	DefaultSceneRoot = CreateDefaultSubobject<USceneComponent>(TEXT("DefaultSceneRoot"));
	RootComponent = DefaultSceneRoot;

	BoxSpawner = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxCollision"));
	BoxSpawner->SetupAttachment(RootComponent);
}
//------------------------------------------------------------------------------------------------------------
void ASpawnerEnemy::BeginPlay()
{
	Super::BeginPlay();
	
	OnSpawner();
}
//------------------------------------------------------------------------------------------------------------
void ASpawnerEnemy::OnSpawner ()
{
	GetWorld()->GetTimerManager().SetTimer(
		SpawnTimerHandle,
		this,
		&ASpawnerEnemy::OnMakeCountEnemies,
		1.0f,
		true
	);
}
//------------------------------------------------------------------------------------------------------------
void ASpawnerEnemy::OnMakeCountEnemies ()
{
	if (EnemiesCount < 80)
	{
		FVector Center = BoxSpawner->GetComponentLocation();
		FVector HalfSize = BoxSpawner->GetScaledBoxExtent();

		FVector SpawnLocation = UKismetMathLibrary::RandomPointInBoundingBox(Center, HalfSize);

		FTransform SpawnTransformParams;
		SpawnTransformParams.SetLocation(SpawnLocation);
		SpawnTransformParams.SetRotation(FQuat::Identity);
		SpawnTransformParams.SetScale3D(FVector(1.0f, 1.0f, 1.0f));

		GetWorld()->SpawnActor<AActor>(SpawnClass, SpawnTransformParams);
	}
}
//------------------------------------------------------------------------------------------------------------
void ASpawnerEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	TArray<AActor*> Enemies;

	UGameplayStatics::GetAllActorsOfClass(GetWorld(), SpawnClass, Enemies);

	EnemiesCount = Enemies.Num();
}
//------------------------------------------------------------------------------------------------------------

