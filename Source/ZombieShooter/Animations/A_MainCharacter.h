#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"

#include "Animation/AnimInstanceProxy.h"
#include "AnimNodes/AnimNode_BlendSpacePlayer.h"
#include "Animation/AnimNode_SequencePlayer.h"
#include "Animation/AnimNode_SaveCachedPose.h"
#include "Animation/AnimNode_UseCachedPose.h"
#include "AnimNodes/AnimNode_LayeredBoneBlend.h"
#include "AnimNodes/AnimNode_BlendListByBool.h"
#include "Animation/AnimNodeSpaceConversions.h"
#include "BoneControllers/AnimNode_ModifyBone.h"
#include "AnimNodes/AnimNode_Slot.h"
#include "BoneControllers/AnimNode_TwoBoneIK.h"
#include "Animation/AnimNode_Root.h"

#include "A_MainCharacter.generated.h"

class ASwatCharacter;

class UBlendSpace;

USTRUCT()
struct FMainCharacterAnimProxy : public FAnimInstanceProxy
{
	GENERATED_BODY()

	FMainCharacterAnimProxy() {}
	FMainCharacterAnimProxy(UAnimInstance* Instance)
		:	FAnimInstanceProxy(Instance){}

	virtual void Initialize(UAnimInstance* Instance) override;
	virtual void PreUpdate(UAnimInstance* Instance, float DeltaSeconds) override;
	virtual void UpdateAnimationNode(const FAnimationUpdateContext& Context) override;
	virtual bool Evaluate(FPoseContext& Output) override;
	virtual FAnimNode_Base* GetCustomRootNode() override;

	FAnimNode_BlendSpacePlayer BS_AimingNode;
	FAnimNode_SequencePlayer RifleAimingIdle;
	FAnimNode_SaveCachedPose MainPosesCached;
	FAnimNode_UseCachedPose MainPoses;
	FAnimNode_LayeredBoneBlend AimingLayer;
	FAnimNode_BlendListByBool AimBlend;
	FAnimNode_ConvertLocalToComponentSpace LocalToComponent;
	FAnimNode_ModifyBone ModifyAimBone;
	FAnimNode_ConvertComponentToLocalSpace ComponentToLocal;
	FAnimNode_SaveCachedPose MovementAndAimingCache;
	FAnimNode_UseCachedPose MovementAndAiming;
	FAnimNode_Slot UpperBodySlot;
	FAnimNode_LayeredBoneBlend UpperBodyLayer;

	// IK 1
	FAnimNode_ConvertLocalToComponentSpace LocalToComponentIK1;
	FAnimNode_TwoBoneIK LeftHandIK1;
	FAnimNode_ConvertComponentToLocalSpace ComponentToLocalIK1;

	// IK 2
	FAnimNode_ConvertLocalToComponentSpace LocalToComponentIK2;
	FAnimNode_TwoBoneIK LeftHandIK2;
	FAnimNode_ConvertComponentToLocalSpace ComponentToLocalIK2;

	FAnimNode_Root Root;
};

UCLASS()
class ZOMBIESHOOTER_API UA_MainCharacter : public UAnimInstance
{
	GENERATED_BODY()
	
public:

	virtual void NativeInitializeAnimation() override;
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;

	virtual FAnimInstanceProxy* CreateAnimInstanceProxy() override;
	virtual void DestroyAnimInstanceProxy(FAnimInstanceProxy* InProxy) override;

	UFUNCTION(BlueprintCallable)
	void OnNativeUpdateAnimation();

	UFUNCTION(BlueprintCallable)
	void OnNativeInitializeAnimation();

	UPROPERTY (BlueprintReadWrite)
	FVector LeftHandSocketPoseAnim;

	UPROPERTY(BlueprintReadWrite)
	bool bAnimIsAiming;

	UPROPERTY(BlueprintReadWrite)
	float CharacterSpeed;

	UPROPERTY(BlueprintReadWrite)
	float CharacterDirection;

	UPROPERTY(BlueprintReadWrite)
	float CharacterAnimOffset;

	UPROPERTY (EditDefaultsOnly, BlueprintReadWrite, Category = "Animations")
	UBlendSpace* BS_AimingAsset;

	FMainCharacterAnimProxy* AnimProxy = nullptr;
	//UPROPERTY(Transient)
	//FMainCharacterAnimProxy AnimProxy;

	UPROPERTY(BlueprintReadWrite)
	ASwatCharacter* SwatCharacterREF;

};

