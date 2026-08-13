#include "FirstAid.h"
#include "SwatCharacter.h"

#include "Components/BoxComponent.h"
#include "Components/SpotLightComponent.h"
#include "GameFramework/RotatingMovementComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Kismet/GameplayStatics.h"

//------------------------------------------------------------------------------------------------------------
AFirstAid::AFirstAid()
{
	PrimaryActorTick.bCanEverTick = true;

	DefaultSceneRootT = CreateDefaultSubobject<USceneComponent>(TEXT("DefaultSceneRoot"));
	RootComponent = DefaultSceneRootT;

	Box = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxCollision"));
	Box->SetupAttachment(RootComponent);
	Box->SetRelativeLocation(FVector(0.0f, 0.0f, 2.729654f));
	Box->SetRelativeScale3D(FVector(0.719402f, 0.469402f, 0.469402f));

	SpotLight = CreateDefaultSubobject<USpotLightComponent>(TEXT("SpotLight"));
	SpotLight->SetupAttachment(RootComponent);
	SpotLight->SetRelativeLocation(FVector(0.0f, 0.0f, 80.0f));
	SpotLight->SetRelativeRotation(FRotator(-90.0f, 0.0f, 0.0f));
	SpotLight->Intensity = 50000.0f;
	SpotLight->LightColor = FColor(0, 0, 255);
	SpotLight->OuterConeAngle = 20.0f;

	StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMesh"));
	StaticMesh->SetupAttachment(RootComponent);
	StaticMesh->SetRelativeScale3D(FVector(0.25f, 0.25f, 0.25f));

	RotatingMovement = CreateDefaultSubobject<URotatingMovementComponent>(TEXT("RotatingMovement"));
}
//------------------------------------------------------------------------------------------------------------
void AFirstAid::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}
//------------------------------------------------------------------------------------------------------------
void AFirstAid::NotifyActorBeginOverlap (AActor* OtherActor)
{
	Super::NotifyActorBeginOverlap(OtherActor);

	if (Cast<ASwatCharacter>(OtherActor))
	{
		AddHealth_Implementation();
	}
}
//------------------------------------------------------------------------------------------------------------
void AFirstAid::AddHealth_Implementation ()
{
	ASwatCharacter* SwatCharacterREF = Cast<ASwatCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));

	if (PickupSound)
	{
		UGameplayStatics::PlaySound2D(this, PickupSound);
	}

	SwatCharacterREF->HealthCharacter += 0.4f;
	SwatCharacterREF->HealthCharacter = FMath::Clamp(SwatCharacterREF->HealthCharacter, 0.0f, 1.0f);
	
	Destroy();
}
//------------------------------------------------------------------------------------------------------------
void AFirstAid::BeginPlay()
{
	Super::BeginPlay();
	
}
//------------------------------------------------------------------------------------------------------------

