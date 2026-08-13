#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "A_Zombie.generated.h"

class AZombie;

UCLASS()
class ZOMBIESHOOTER_API UA_Zombie : public UAnimInstance
{
	GENERATED_BODY()
	
public:

	virtual void NativeInitializeAnimation() override;
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;

	UPROPERTY(BlueprintReadWrite)
	AZombie* ZombieREF;

	UPROPERTY(BlueprintReadWrite)
	bool bIsGetCharacterMy;

};
