#include "SwatCharacter.h"
#include "Weapon.h"
#include "Widgets/MainWidget.h"
#include "Widgets/W_Croshair.h"

#include "Components/ArrowComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "Blueprint/UserWidget.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "Components/ProgressBar.h"
#include "Blueprint/WidgetLayoutLibrary.h"
#include "Components/TextBlock.h"
#include "Camera/PlayerCameraManager.h"
#include "Camera/CameraShakeSourceComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Animation/WidgetAnimation.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"

//------------------------------------------------------------------------------------------------------------
ASwatCharacter::ASwatCharacter()
{
	PrimaryActorTick.bCanEverTick = true;

	UCapsuleComponent* DefaultCapsule = GetCapsuleComponent();
	RootComponent = DefaultCapsule;
	DefaultCapsule->SetCapsuleHalfHeight(97.0f);
	DefaultCapsule->SetCapsuleRadius(35.0f);
	DefaultCapsule->SetCollisionProfileName(TEXT("Pawn"));
	DefaultCapsule->CanCharacterStepUpOn = ECanBeCharacterBase::ECB_No;
	DefaultCapsule->bDynamicObstacle = true;

	UArrowComponent* DefaultArrow = GetArrowComponent();

	USkeletalMeshComponent* DefaultMesh = GetMesh();
	DefaultMesh->SetupAttachment(RootComponent);
	DefaultMesh->SetRelativeLocation(FVector(0.0f, 0.0f, -97.0f));
	DefaultMesh->SetRelativeRotation(FRotator(0.0f, -90.0f, 0.0f));
	DefaultMesh->SetCollisionProfileName(TEXT("CharacterMesh"));

	SpringArmComponent = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	SpringArmComponent->SetupAttachment(RootComponent);
	SpringArmComponent->TargetArmLength = 200.0f;
	SpringArmComponent->SocketOffset = FVector(0.0f, 70.0f, 0.0f);
	SpringArmComponent->bUsePawnControlRotation = true;

	CameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	CameraComponent->SetupAttachment(SpringArmComponent);

	UCharacterMovementComponent* DefaultMovementComponent = GetCharacterMovement();
	DefaultMovementComponent->MaxWalkSpeed = 200.0f;
	DefaultMovementComponent->RotationRate = FRotator(0.0f, 600.0f, 0.0f);
	DefaultMovementComponent->bOrientRotationToMovement = true;
}
//------------------------------------------------------------------------------------------------------------
void ASwatCharacter::BeginPlay()
{
	Super::BeginPlay();

	SpawnWeapon();
	SetPlayerController();
	SpawnWidget();
}
//------------------------------------------------------------------------------------------------------------
void ASwatCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	LeftHandSocketPoseCharacter = WeaponREF->WeaponMesh->GetSocketLocation(LeftHandSocketPos);
	
	if (AmmoCharacter == 0)
	{
		bCanShootMY = false;
	}
	else
	{
		bCanShootMY = true;
	}

	FText AmmoText = FText::AsNumber(AmmoCharacter);

	MainWidgetREF->AmmoTextBlock->SetText(AmmoText);

	TLAim.TickTimeline(DeltaTime);
}
//------------------------------------------------------------------------------------------------------------
void ASwatCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	UEnhancedInputComponent* EnhancedInputComponent = 
		CastChecked<UEnhancedInputComponent>(PlayerInputComponent);

	EnhancedInputComponent->BindAction(IA_Move, ETriggerEvent::Triggered, this, &ThisClass::Move);
	EnhancedInputComponent->BindAction(IA_Look, ETriggerEvent::Triggered, this, &ThisClass::Look);

	EnhancedInputComponent->BindAction(IA_Jump, ETriggerEvent::Started, this, &ThisClass::Jump);
	EnhancedInputComponent->BindAction(IA_Jump, ETriggerEvent::Completed, this, &ThisClass::StopJumping);

	EnhancedInputComponent->BindAction(IA_Sprint, ETriggerEvent::Triggered, this, &ThisClass::SprintTriggered);
	EnhancedInputComponent->BindAction(IA_Sprint, ETriggerEvent::Canceled, this, &ThisClass::SprintCanceled);
	EnhancedInputComponent->BindAction(IA_Sprint, ETriggerEvent::Completed, this, &ThisClass::SprintCompleted);

	EnhancedInputComponent->BindAction(IA_Aiming, ETriggerEvent::Started, this, &ThisClass::IaAimingStarted);
	EnhancedInputComponent->BindAction(IA_Aiming, ETriggerEvent::Canceled, this, &ThisClass::IaAimingCanceledAndCompleted);
	EnhancedInputComponent->BindAction(IA_Aiming, ETriggerEvent::Completed, this, &ThisClass::IaAimingCanceledAndCompleted);

	EnhancedInputComponent->BindAction(IA_Shoot, ETriggerEvent::Triggered, this, &ThisClass::OnOnceShoot);
	EnhancedInputComponent->BindAction(IA_Shoot, ETriggerEvent::Completed, this, &ThisClass::OnStopShoot);

	EnhancedInputComponent->BindAction(IA_ThrowGrenade, ETriggerEvent::Started, this, &ThisClass::SpawnGrenade);
}
//------------------------------------------------------------------------------------------------------------
void ASwatCharacter::PostInitializeComponents ()
{
	Super::PostInitializeComponents();

	TLAimInitialize();
}
//------------------------------------------------------------------------------------------------------------
float ASwatCharacter::TakeDamage (float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	OnTakeDamage(DamageAmount);

	return 0.0f;
}
//------------------------------------------------------------------------------------------------------------
void ASwatCharacter::OnTakeDamage (float Damage)
{
	OnHitEffect();

	HealthCharacter = FMath::Clamp(HealthCharacter - Damage, 0.0f, 1.0f);

	MainWidgetREF->HealthCharacterProgressBar->SetPercent(HealthCharacter);

	if (HealthCharacter <= 0.0f)
	{
		GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);

		GetMesh()->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
		GetMesh()->SetSimulatePhysics(true);
		UWidgetLayoutLibrary::RemoveAllWidgets(GetWorld());

		GetWorld()->GetTimerManager().SetTimer(
			DelayTakeDamageTimer,
			this,
			&ASwatCharacter::OnCreateDeathWidget,
			0.2f,
			false
		);
	}
}
//------------------------------------------------------------------------------------------------------------
void ASwatCharacter::OnHitEffect ()
{
	CameraComponent->PostProcessSettings = HitPostProcessSettings;
	CameraComponent->PostProcessBlendWeight = 1.0f;

	GetWorld()->GetTimerManager().SetTimer(
		DelayPostProcessTimer,
		this,
		&ASwatCharacter::DelayHitEffect,
		0.3f,
		false
	);
}
//------------------------------------------------------------------------------------------------------------
void ASwatCharacter::SpawnGrenade ()
{
	if (GrenadeAmount > 0)
	{
		GrenadeAmount -= 1;
		GrenadeAmount = FMath::Clamp(GrenadeAmount, GrenadeMin, GrenadeMax);

		WeaponREF = Cast<AWeapon>(UGameplayStatics::GetActorOfClass(GetWorld(), AWeapon::StaticClass()));

		FTransform Transform = WeaponREF->ArrowComponent->GetSocketTransform(NAME_None, ERelativeTransformSpace::RTS_World);

		FActorSpawnParameters Params;
		Params.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;

		AActor* NewActor = GetWorld()->SpawnActor<AActor>(
			GrenadeClass,
			Transform,
			Params
		);
	}

	FText GrenadeText = FText::AsNumber(GrenadeAmount);

	MainWidgetREF->GrenadeCounterText->SetText(GrenadeText);
}
//------------------------------------------------------------------------------------------------------------
void ASwatCharacter::OnOnceShoot ()
{
	if (!GetWorld()->GetTimerManager().IsTimerActive(DelayOnShootTimer))
	{
		//SoundEmptyWeaponAndFiringMontage();

		GetWorld ()->GetTimerManager ().SetTimer (
			DelayOnShootTimer,
			this,
			&ASwatCharacter::SoundEmptyWeaponAndFiringMontage,
			0.05f,
			false
		);
	}
}
//------------------------------------------------------------------------------------------------------------
void ASwatCharacter::OnStopShoot ()
{
	GetWorld()->GetTimerManager().ClearTimer(DelayOnShootTimer);

	if (FiringMontage)
	{
		StopAnimMontage(FiringMontage);
	}
}
//------------------------------------------------------------------------------------------------------------
void ASwatCharacter::Move (const FInputActionValue& Value)
{
	FVector2D Input = Value.Get<FVector2D>();

	FRotator ControlRotationFirst = GetControlRotation();
	ControlRotationFirst.Pitch = 0.0f;

	FVector WorldDirectionFirst = UKismetMathLibrary::GetRightVector(ControlRotationFirst);

	AddMovementInput(WorldDirectionFirst, Input.X);

	FRotator ControlRotationSecond = GetControlRotation();
	ControlRotationSecond.Roll = 0.0f;
	ControlRotationSecond.Pitch = 0.0f;

	FVector WorldDirectionSecond = UKismetMathLibrary::GetForwardVector(ControlRotationSecond);

	AddMovementInput(WorldDirectionSecond, Input.Y);
}
//------------------------------------------------------------------------------------------------------------
void ASwatCharacter::Look (const FInputActionValue& Value)
{
	FVector2D Input = Value.Get<FVector2D>();

	AddControllerYawInput(Input.X);
	AddControllerPitchInput(Input.Y);
}
//------------------------------------------------------------------------------------------------------------
void ASwatCharacter::SprintTriggered ()
{
	GetCharacterMovement()->MaxWalkSpeed = 400.0f;
}
//------------------------------------------------------------------------------------------------------------
void ASwatCharacter::SprintCanceled ()
{
	GetCharacterMovement()->MaxWalkSpeed = 200.0f;
}
//------------------------------------------------------------------------------------------------------------
void ASwatCharacter::SprintCompleted ()
{
	GetCharacterMovement()->MaxWalkSpeed = 200.0f;
}
//------------------------------------------------------------------------------------------------------------
void ASwatCharacter::IaAimingStarted ()
{
	bIsAimingMY = true;

	GetCharacterMovement()->bOrientRotationToMovement = false;

	bUseControllerRotationYaw = true;

	W_CroshairREF->AddToViewport();

	TLAim.Play();

	W_CroshairREF->PlayAnimationForward(W_CroshairREF->AnimCroshair, 1.0f, false);
}
//------------------------------------------------------------------------------------------------------------
void ASwatCharacter::IaAimingCanceledAndCompleted ()
{
	bIsAimingMY = false;

	GetCharacterMovement()->bOrientRotationToMovement = true;

	bUseControllerRotationYaw = false;

	W_CroshairREF->RemoveFromParent();

	TLAim.Reverse();
}
//------------------------------------------------------------------------------------------------------------
void ASwatCharacter::DelayHitEffect ()
{
	CameraComponent->PostProcessSettings = HitPostProcessSettings;
	CameraComponent->PostProcessBlendWeight = 0.0f;
}
//------------------------------------------------------------------------------------------------------------
void ASwatCharacter::WeaponAmmoCounter ()
{
	AmmoCharacter -= 1;
	AmmoCharacter = FMath::Clamp(AmmoCharacter, AmmoMin, AmmoMax);
}
//------------------------------------------------------------------------------------------------------------
void ASwatCharacter::SetPlayerController ()
{
	APlayerController* PlayerController = Cast<APlayerController>(GetController());

	if (PlayerController)
	{
		ULocalPlayer* LocalPlayer = PlayerController->GetLocalPlayer();

		if (LocalPlayer)
		{
			UEnhancedInputLocalPlayerSubsystem* Subsystem = 
				LocalPlayer->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>();

			if (Subsystem)
			{
				Subsystem->AddMappingContext(IMC_Default, 0);
			}
		}
	}
}
//------------------------------------------------------------------------------------------------------------
void ASwatCharacter::SpawnWeapon ()
{
	FTransform Transform = GetMesh()->GetSocketTransform(WeaponSocket, ERelativeTransformSpace::RTS_World);

	FActorSpawnParameters SpawnParams;

	FAttachmentTransformRules AttachRules (
		EAttachmentRule::SnapToTarget, 
		EAttachmentRule::SnapToTarget,
		EAttachmentRule::SnapToTarget,
		true
	);

	WeaponREF = GetWorld()->SpawnActor<AWeapon>(WeaponClass, Transform, SpawnParams);

	if (WeaponREF)
	{
		WeaponREF->AttachToComponent(GetMesh(), AttachRules, WeaponSocket);
	}
}
//------------------------------------------------------------------------------------------------------------
void ASwatCharacter::SpawnWidget ()
{
	OnCreateCrosshairWidget();
	OnCreateMainWidget();
}
//------------------------------------------------------------------------------------------------------------
void ASwatCharacter::OnCreateMainWidget ()
{
	if (MainWidgetClass)
	{
		MainWidgetREF = CreateWidget<UMainWidget>(GetWorld(), MainWidgetClass);

		if (MainWidgetREF)
		{
			MainWidgetREF->AddToViewport();
		}
	}
}
//------------------------------------------------------------------------------------------------------------
void ASwatCharacter::OnCreateCrosshairWidget ()
{
	if (CrosshairWidgetClass)
	{
		W_CroshairREF = CreateWidget<UW_Croshair>(GetWorld(), CrosshairWidgetClass);
	}
}
//------------------------------------------------------------------------------------------------------------
void ASwatCharacter::SoundEmptyWeaponAndFiringMontage ()
{	
	if (bIsAimingMY)
	{
		if (bCanShootMY)
		{
			if (FiringMontage)
			{
				UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
				
				AnimInstance->Montage_Play(FiringMontage, 1.0f);
			}

			WeaponREF->SpawnLightWithDelay();

			float PitchSound = FMath::FRandRange (0.8f, 1.1f);

			UGameplayStatics::SpawnSound2D (
				this,
				SoundFireWeapon,
				1.0f,
				PitchSound
			);

			FTransform RelativeTransform = CameraComponent->GetRelativeTransform();

			UCameraShakeSourceComponent* ReturnValue = Cast<UCameraShakeSourceComponent>(AddComponentByClass(
					UCameraShakeSourceComponent::StaticClass(), false, RelativeTransform, false));

			WeaponAmmoCounter();

			OnLineTrace();
		}
		else 
		{
			if (SoundEmptyWeapon)
			{
				float PitchSound = FMath::FRandRange (0.8f, 1.1f);

				UGameplayStatics::SpawnSound2D (
					this,
					SoundEmptyWeapon,
					1.0f,
					PitchSound
				);
				return;
			}
		}
	}
}
//------------------------------------------------------------------------------------------------------------
void ASwatCharacter::OnCreateDeathWidget ()
{
	if (DeathWidgetClass)
	{
		DeathWidget = CreateWidget<UUserWidget>(GetWorld(), DeathWidgetClass);

		if (DeathWidget)
		{
			DeathWidget->AddToViewport();

			APlayerController* PC = UGameplayStatics::GetPlayerController(GetWorld(), 0);

			PC->bShowMouseCursor = true;

			FInputModeUIOnly InputMode;
			InputMode.SetWidgetToFocus(DeathWidget->TakeWidget());
			InputMode.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
			PC->SetInputMode(InputMode);

			UGameplayStatics::SetGamePaused(GetWorld(), true);
		}
	}
}
//------------------------------------------------------------------------------------------------------------
void ASwatCharacter::OnLineTrace ()
{
	FHitResult HitResult;

	APlayerCameraManager* CameraManager = UGameplayStatics::GetPlayerCameraManager(GetWorld(), 0);
	
	FVector StartLocation = CameraManager->GetCameraLocation();
	FVector EndLocation = StartLocation + CameraManager->GetActorForwardVector() * 50000;

	FCollisionQueryParams ParamsBegin;
	ParamsBegin.bTraceComplex = false;
	ParamsBegin.AddIgnoredActor(this);

	bool bHit = GetWorld()->LineTraceSingleByChannel(
		HitResult,
		StartLocation,
		EndLocation,
		ECollisionChannel::ECC_Visibility,
		ParamsBegin
	);

	FVector ImpactPoint = HitResult.ImpactPoint;
	AActor* HitActor = HitResult.GetActor();
	FName HitBoneName = HitResult.BoneName;
	FVector TraceEnd = HitResult.TraceEnd;

	if (bHit)
	{
		FHitResult HitResultSuccess;

		FVector StartLocationSuccess = WeaponREF->ArrowComponent->GetComponentLocation();
		FVector EndLocationSuccess = ImpactPoint;

		FCollisionQueryParams ParamsSuccess;
		ParamsSuccess.bTraceComplex = false;
		ParamsSuccess.AddIgnoredActor(this);

		GetWorld()->LineTraceSingleByChannel(
			HitResultSuccess,
			StartLocationSuccess,
			EndLocationSuccess,
			ECollisionChannel::ECC_Visibility,
			ParamsSuccess
		);

		if (HitActor->ActorHasTag(TEXT("Enemy")))
		{
			UGameplayStatics::SpawnSoundAtLocation(
				GetWorld(),
				SoundHitZombie,
				ImpactPoint,
				FRotator::ZeroRotator,
				1.0f,
				FMath::FRandRange(0.7f, 1.3f),
				0.0f,
				SA_HitSound
			);

			float RandomFloat = FMath::FRandRange(0.4f, 1.0f);

			UGameplayStatics::SpawnEmitterAtLocation(
				GetWorld(),
				P_ImpactBlood,
				ImpactPoint,
				FRotator(RandomFloat, RandomFloat, RandomFloat),
				FVector(RandomFloat, RandomFloat, RandomFloat)
			);

			float BaseDamage = FMath::FRandRange(0.1f, 0.4f);

			UGameplayStatics::ApplyDamage(
				HitActor,
				BaseDamage,
				nullptr,
				this,
				UDamageType::StaticClass()
			);

			FName Head = "Head";

			if (HitBoneName == Head)
			{
				FHitResult HitInfo;
				HitInfo.bBlockingHit = false;
				HitInfo.BoneName = HitBoneName;

				UGameplayStatics::ApplyPointDamage(
					HitActor,
					1.0f,
					ImpactPoint,
					HitInfo,
					nullptr,
					nullptr,
					nullptr
				);
			}
		}
		else
		{
			UGameplayStatics::SpawnSoundAtLocation(
				GetWorld(),
				SoundHitMaterial,
				ImpactPoint,
				FRotator::ZeroRotator,
				0.2f,
				FMath::FRandRange(0.7f, 1.3f),
				0.0f,
				SA_HitSound
			);

			int RandomIndex = FMath::RandRange(0, 2);

			switch (RandomIndex)
			{
				case 0:
					SelectedEmitter = P_ImpactPlaster;
					break;
				case 1:
					SelectedEmitter = P_ImpactBrick;
					break;
				case 2:
					SelectedEmitter = P_ImpactConcrete;
					break;
			}

			float RandomFloat = FMath::FRandRange(0.4f, 1.0f);

			UGameplayStatics::SpawnEmitterAtLocation(
				GetWorld(),
				SelectedEmitter,
				ImpactPoint,
				FRotator(RandomFloat, RandomFloat, RandomFloat),
				FVector(RandomFloat, RandomFloat, RandomFloat)
			);
		}
	}
	else
	{
		FHitResult HitResultFail;

		FVector StartLocationFail = WeaponREF->ArrowComponent->GetComponentLocation();
		FVector EndLocationFail = TraceEnd;

		FCollisionQueryParams ParamsFail;
		ParamsFail.bTraceComplex = false;
		ParamsFail.AddIgnoredActor(this);

		GetWorld()->LineTraceSingleByChannel(
			HitResultFail,
			StartLocationFail,
			EndLocationFail,
			ECollisionChannel::ECC_Visibility,
			ParamsFail
		);
	}
}
//------------------------------------------------------------------------------------------------------------
void ASwatCharacter::TLAimUpdate (float Zoom)
{
	FVector CurrentLocation = CameraComponent->GetRelativeLocation();

	CurrentLocation.X = FMath::Lerp(0.0f, 50.0f, Zoom);

	CameraComponent->SetRelativeLocation(FVector(
		CurrentLocation.X,
		CurrentLocation.Y,
		CurrentLocation.Z
	));
}
//------------------------------------------------------------------------------------------------------------
void ASwatCharacter::TLAimInitialize ()
{
	AimCurve = NewObject<UCurveFloat>(this);
	FRichCurve& FloatCurve = AimCurve->FloatCurve;

	FloatCurve.AddKey(0.0f, 0.0f);
	FloatCurve.AddKey(1.0f, 1.0f);

	for (FRichCurveKey& Key : FloatCurve.Keys)
	{
		Key.InterpMode = RCIM_Linear;
	}

	FOnTimelineFloat TimeLineUpdate;
	TimeLineUpdate.BindUFunction(this, FName("TLAimUpdate"));

	TLAim.AddInterpFloat(AimCurve, TimeLineUpdate);
	TLAim.SetTimelineLength(1.0f);
	TLAim.SetLooping(false);
}
//------------------------------------------------------------------------------------------------------------
