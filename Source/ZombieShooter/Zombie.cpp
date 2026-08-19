#include "Zombie.h"
#include "Widgets/W_ZombieHealthBar.h"
#include "SwatCharacter.h"

#include "Blueprint/UserWidget.h"
#include "Components/CapsuleComponent.h"
#include "Components/WidgetComponent.h"
#include "Components/ProgressBar.h"
#include "Components/ArrowComponent.h"
#include "Components/SphereComponent.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "AIController.h"

//------------------------------------------------------------------------------------------------------------
AZombie::AZombie()
{
	PrimaryActorTick.bCanEverTick = true;

	UCapsuleComponent* DefaultCapsule = GetCapsuleComponent();
	DefaultCapsule->SetCapsuleHalfHeight(64.0f);
	DefaultCapsule->SetCapsuleRadius(34.0f);
	DefaultCapsule->CanCharacterStepUpOn = ECanBeCharacterBase::ECB_No;
	DefaultCapsule->SetCollisionProfileName(TEXT("Custom"));
	DefaultCapsule->SetCollisionObjectType(ECC_Pawn);
	DefaultCapsule->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	DefaultCapsule->bDynamicObstacle = true;
	DefaultCapsule->SetCanEverAffectNavigation(false);

	USkeletalMeshComponent* DefaultMesh = GetMesh();
	DefaultMesh->SetupAttachment(RootComponent);
	DefaultMesh->SetRelativeLocation(FVector(0.0f, 0.0f, -66.401313f));
	DefaultMesh->SetRelativeRotation(FRotator(0.0f, -90.0f, 0.0f));
	DefaultMesh->SetRelativeScale3D(FVector(0.768813f));
	DefaultMesh->SetCollisionProfileName(TEXT("CharacterMesh"));

	HitSphere = CreateDefaultSubobject<USphereComponent>(TEXT("HitSphere"));
	HitSphere->SetupAttachment(GetMesh(), FName("RightHandThumb1"));
	HitSphere->SetRelativeScale3D(FVector(0.795028f));
	HitSphere->SetSphereRadius(SphereRadius);

	WidgetZombieHealth = CreateDefaultSubobject<UWidgetComponent>(TEXT("Zombie Health"));
	WidgetZombieHealth->SetupAttachment(RootComponent);
	WidgetZombieHealth->SetRelativeLocation(FVector(0.0f, 0.0f, 77.859969f));
	WidgetZombieHealth->SetCollisionProfileName(TEXT("Custom"));
	WidgetZombieHealth->SetDrawSize(FVector2D(80.0f, 5.0f));
}
//------------------------------------------------------------------------------------------------------------
void AZombie::BeginPlay()
{
	Super::BeginPlay();

	OnMoveToCharacter();

	W_ZombieHealthBarREF = Cast<UW_ZombieHealthBar>(WidgetZombieHealth->GetUserWidgetObject());
}
//------------------------------------------------------------------------------------------------------------
void AZombie::OnTakeDamage (float Damage)
{
	HealthZombie = FMath::Clamp(HealthZombie - Damage, 0.0f, 1.0f);

	W_ZombieHealthBarREF->HealthZombieProgressBar->SetPercent(HealthZombie);

	if (HealthZombie <= 0)
	{
		UCapsuleComponent* Capsule = GetCapsuleComponent();
		if (Capsule)
		{
			Capsule->SetSimulatePhysics (true);
		}

		if (IsValid(WidgetZombieHealth))
		{
			WidgetZombieHealth->DestroyComponent();

		}

		Destroy();
	}
}
//------------------------------------------------------------------------------------------------------------
void AZombie::OnGetHit ()
{
	FVector Location = HitSphere->GetComponentLocation();
	float Radius = SphereRadius;
	TArray<TEnumAsByte<EObjectTypeQuery>> ObjectTypes;
	ObjectTypes.Add(UEngineTypes::ConvertToObjectType(ECC_Pawn));
	TArray<AActor*> OutActors;

	UKismetSystemLibrary::SphereOverlapActors(
		GetWorld(),
		Location,
		Radius,
		ObjectTypes,
		AActor::StaticClass(),
		TArray<AActor*>(),
		OutActors
	);

	for (AActor* CurrentActor : OutActors)
	{
		if (CurrentActor == this)
		{
			continue;
		}

		UGameplayStatics::ApplyDamage(CurrentActor, BaseDamage, nullptr, nullptr, nullptr);
	}
}
//------------------------------------------------------------------------------------------------------------
void AZombie::OnMoveToCharacter ()
{
	AAIController* AIController = Cast<AAIController>(GetController());
	if (AIController)
	{
		AIController->ReceiveMoveCompleted.RemoveDynamic(this, &AZombie::OnMoveFinished);
		AIController->ReceiveMoveCompleted.AddDynamic(this, &AZombie::OnMoveFinished);
	}

	GetWorld()->GetTimerManager().SetTimer(
		MoveToCharacterTimer,
		this,
		&AZombie::OnMoveToCharacterUpdate,
		0.1f,
		false
	);
}
//------------------------------------------------------------------------------------------------------------
void AZombie::OnMoveToCharacterUpdate ()
{
	ASwatCharacter* TargetActor = Cast<ASwatCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));

	AAIController* AIController = Cast<AAIController>(GetController());
	if (AIController)
	{
		FAIMoveRequest MoveRequest;
		MoveRequest.SetGoalActor(TargetActor);
		MoveRequest.SetAcceptanceRadius(20.0f);

		FNavPathSharedPtr NavPath;
		AIController->MoveTo (MoveRequest, &NavPath);
	}
}
//------------------------------------------------------------------------------------------------------------
void AZombie::ResetGetCharacter ()
{
	bIsGetCharacterMY = false;

	GetWorld()->GetTimerManager().ClearTimer(MoveToCharacterTimer);

	OnMoveToCharacter();
}
//------------------------------------------------------------------------------------------------------------
FVector AZombie::OnFuncMoveToCharacter ()
{
	ASwatCharacter* SwatCharacterREF = Cast<ASwatCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));

	if (SwatCharacterREF)
	{
		return SwatCharacterREF->GetActorLocation();
	}

	return FVector::ZeroVector;
}
//------------------------------------------------------------------------------------------------------------
void AZombie::OnMoveFinished (FAIRequestID RequestID, EPathFollowingResult::Type Result)
{
	if (Result== EPathFollowingResult::Success)
	{
		bIsGetCharacterMY = true;
		
		GetWorld()->GetTimerManager().ClearTimer(MoveToCharacterTimer);

		if (GetMesh() && ZombieAttackMontage)
		{
			UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();

			if (AnimInstance)
			{
				float Duration = AnimInstance->Montage_Play(ZombieAttackMontage, 1.0f);

				GetWorld()->GetTimerManager().SetTimer(
					HitTimer,
					this,
					&AZombie::OnGetHit,
					Duration * 0.5f,
					false
				);

				GetWorld()->GetTimerManager().SetTimer(
					ResetTimer,
					this,
					&AZombie::OnMoveToCharacter,
					Duration,
					false
				);
			}
		}
	}
	else
	{
		GetWorld()->GetTimerManager().SetTimer(
			ResetTimer,
			this,
			&AZombie::ResetGetCharacter,
			1.0f,
			true
		);
	}
}
//------------------------------------------------------------------------------------------------------------
float AZombie::TakeDamage (float DamageAmount, FDamageEvent const& DamageEvent, 
	AController* EventInstigator, AActor* DamageCauser)
{
	OnTakeDamage(DamageAmount);
	
	return 0.0f;
}
//------------------------------------------------------------------------------------------------------------
float AZombie::TakeRadialDamage (float DamageAmount, FRadialDamageEvent const& RadialDamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	OnTakeDamage(DamageAmount);
	
	return 0.0f;
}
//------------------------------------------------------------------------------------------------------------
void AZombie::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}
//------------------------------------------------------------------------------------------------------------
void AZombie::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}
//------------------------------------------------------------------------------------------------------------
