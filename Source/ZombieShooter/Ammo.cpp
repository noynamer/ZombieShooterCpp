#include "Ammo.h"
#include "SwatCharacter.h"

#include "Components/BoxComponent.h"
#include "Components/SpotLightComponent.h"
#include "GameFramework/RotatingMovementComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Kismet/GameplayStatics.h"

//------------------------------------------------------------------------------------------------------------
AAmmo::AAmmo()
{
	PrimaryActorTick.bCanEverTick = true;

	DefaultSceneRoot = CreateDefaultSubobject<USceneComponent>(TEXT("DefaultSceneRoot"));
	RootComponent = DefaultSceneRoot;

	Box = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxCollision"));
	Box->SetupAttachment(RootComponent);
	Box->SetRelativeLocation(FVector(0.0f, 0.0f, 2.729654f));
	Box->SetRelativeScale3D(FVector(0.469402f, 0.469402f, 0.469402f));

	SpotLight = CreateDefaultSubobject<USpotLightComponent>(TEXT("SpotLight"));
	SpotLight->SetupAttachment(RootComponent);
	SpotLight->SetRelativeLocation(FVector(0.0f, 0.0f, 80.0f));
	SpotLight->SetRelativeRotation(FRotator(-90.0f, 0.0f, 0.0f));
	SpotLight->Intensity = 50000.0f;
	SpotLight->LightColor = FColor(0, 0, 255);
	SpotLight->OuterConeAngle = 20.0f;

	StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMesh"));
	StaticMesh->SetupAttachment(RootComponent);

	RotatingMovement = CreateDefaultSubobject<URotatingMovementComponent>(TEXT("RotatingMovement"));
}
//------------------------------------------------------------------------------------------------------------
void AAmmo::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}
//------------------------------------------------------------------------------------------------------------
void AAmmo::NotifyActorBeginOverlap (AActor* OtherActor)
{
	Super::NotifyActorBeginOverlap(OtherActor);

	if (Cast<ASwatCharacter>(OtherActor))
	{
		AddAmmo_Implementation();
	}
}
//------------------------------------------------------------------------------------------------------------
void AAmmo::AddAmmo_Implementation()
{
	ASwatCharacter* SwatCharacterREF = Cast<ASwatCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));

	if (PickupSound)
	{
		UGameplayStatics::PlaySound2D(this, PickupSound);
	}

	SwatCharacterREF->AmmoCharacter += 60.0f;
	SwatCharacterREF->AmmoCharacter = FMath::Clamp(
		SwatCharacterREF->AmmoCharacter, SwatCharacterREF->AmmoMin, SwatCharacterREF->AmmoMax);

	Destroy();
}
//------------------------------------------------------------------------------------------------------------
void AAmmo::BeginPlay()
{
	Super::BeginPlay();
}
//------------------------------------------------------------------------------------------------------------
