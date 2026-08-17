#include "A_MainCharacter.h"
#include "../SwatCharacter.h"

#include "Kismet/KismetMathLibrary.h"
#include "Kismet/GameplayStatics.h"

//------------------------------------------------------------------------------------------------------------
void UA_MainCharacter::NativeInitializeAnimation ()
{
	Super::NativeInitializeAnimation();

	SwatCharacterREF = Cast<ASwatCharacter>(TryGetPawnOwner());
}
//------------------------------------------------------------------------------------------------------------
void UA_MainCharacter::NativeUpdateAnimation (float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	if (!TryGetPawnOwner()) return;

	CharacterSpeed = TryGetPawnOwner()->GetVelocity().Size();

	if (IsValid(SwatCharacterREF))
	{
		LeftHandSocketPoseAnim = SwatCharacterREF->LeftHandSocketPoseCharacter;

		FVector Velocity = SwatCharacterREF->GetVelocity();
		FRotator BaseRotation = SwatCharacterREF->GetActorRotation();

		CharacterDirection = CalculateDirection(Velocity, BaseRotation);

		if (bAnimIsAiming)
		{
			FRotator AnimRotation = SwatCharacterREF->GetBaseAimRotation();
			FRotator ActorRotation = SwatCharacterREF->GetActorRotation();

			FRotator DeltaRotator = UKismetMathLibrary::NormalizedDeltaRotator(AnimRotation, ActorRotation);
			DeltaRotator.Pitch *= -1;

			CharacterAnimOffset = FMath::Clamp(DeltaRotator.Pitch, -55, 55);
		}
		else
		{
			CharacterAnimOffset = 0.0f;
		}
	}

	if (IsValid(SwatCharacterREF))
	{
		bAnimIsAiming = SwatCharacterREF->bIsAimingMY;
	}
}
//------------------------------------------------------------------------------------------------------------
