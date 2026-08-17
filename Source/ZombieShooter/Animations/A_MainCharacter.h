#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "A_MainCharacter.generated.h"

class ASwatCharacter;

UCLASS()
class ZOMBIESHOOTER_API UA_MainCharacter : public UAnimInstance
{
	GENERATED_BODY()
	
public:

	virtual void NativeInitializeAnimation() override;
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;

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

	UPROPERTY(BlueprintReadWrite)
	ASwatCharacter* SwatCharacterREF;

};
