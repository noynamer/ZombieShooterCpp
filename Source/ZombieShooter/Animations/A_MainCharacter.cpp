#include "A_MainCharacter.h"
#include "../SwatCharacter.h"

#include "Kismet/KismetMathLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "AnimNodes/AnimNode_BlendListByBool.h"

// struct FMainCharacterAnimProxy
//------------------------------------------------------------------------------------------------------------
void FMainCharacterAnimProxy::Initialize (UAnimInstance* Instance)
{
	Super::Initialize(Instance);

	UA_MainCharacter* Anim = Cast<UA_MainCharacter>(Instance);

	if (Anim->BS_AimingAsset)
	{
		BS_AimingNode.SetBlendSpace(Anim->BS_AimingAsset);
		BS_AimingNode.SetLoop(true);
	}

	MainPosesCache.CachePoseName = TEXT("MainPoses");
	MainPoses.LinkToCachingNode = &MainPosesCache;
}
//------------------------------------------------------------------------------------------------------------
void FMainCharacterAnimProxy::PreUpdate (UAnimInstance* Instance, float DeltaSeconds)
{
	Super::PreUpdate(Instance, DeltaSeconds);

	UA_MainCharacter* Anim = Cast<UA_MainCharacter>(Instance);

	BS_AimingNode.SetPosition(FVector(
		Anim->CharacterDirection,
		Anim->CharacterSpeed,
		0.0f
	));

	AimBlend.bActiveValue = Anim->bAnimIsAiming;

	ModifyAimBone.Rotation = FRotator(
		Anim->CharacterAnimOffset,
		0.0f,
		0.0f
	);

	LeftHandIK1.EffectorLocation = Anim->LeftHandSocketPoseAnim;
	LeftHandIK2.EffectorLocation = Anim->LeftHandSocketPoseAnim;
}
//------------------------------------------------------------------------------------------------------------
void FMainCharacterAnimProxy::UpdateAnimationNode (const FAnimationUpdateContext& Context)
{
}
//------------------------------------------------------------------------------------------------------------
bool FMainCharacterAnimProxy::Evaluate (FPoseContext& Output)
{
	return false;
}
//------------------------------------------------------------------------------------------------------------
FAnimNode_Base* FMainCharacterAnimProxy::GetCustomRootNode ()
{
	return nullptr;
}
//------------------------------------------------------------------------------------------------------------




// class UA_MainCharacter
//------------------------------------------------------------------------------------------------------------
void UA_MainCharacter::NativeInitializeAnimation ()
{
	Super::NativeInitializeAnimation();

	//if (BS_AimingAsset)
	//{
	//	BS_AimingNode.SetBlendSpace(BS_AimingAsset);
	//	BS_AimingNode.SetLoop(true);
	//}

}
//------------------------------------------------------------------------------------------------------------
void UA_MainCharacter::NativeUpdateAnimation (float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	//FVector BSPosition = FVector(CharacterDirection, CharacterSpeed, 0.0f);

	//BS_AimingNode.SetPosition(BSPosition);
}
//------------------------------------------------------------------------------------------------------------
FAnimInstanceProxy* UA_MainCharacter::CreateAnimInstanceProxy ()
{
	return &AnimProxy;
	//return new FMainCharacterAnimProxy(this);
}
//------------------------------------------------------------------------------------------------------------
void UA_MainCharacter::DestroyAnimInstanceProxy (FAnimInstanceProxy* InProxy)
{
}
//------------------------------------------------------------------------------------------------------------
void UA_MainCharacter::OnNativeUpdateAnimation ()
{
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
void UA_MainCharacter::OnNativeInitializeAnimation ()
{
	SwatCharacterREF = Cast<ASwatCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
}
//------------------------------------------------------------------------------------------------------------
