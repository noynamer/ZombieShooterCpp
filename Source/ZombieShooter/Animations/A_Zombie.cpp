#include "A_Zombie.h"
#include "../Zombie.h"

#include "Kismet/GameplayStatics.h"

//------------------------------------------------------------------------------------------------------------
void UA_Zombie::NativeInitializeAnimation ()
{
	Super::NativeInitializeAnimation();

	AActor* CurrentActor = UGameplayStatics::GetActorOfClass(GetWorld(), AZombie::StaticClass());

	ZombieREF = Cast<AZombie>(CurrentActor);
}
//------------------------------------------------------------------------------------------------------------
void UA_Zombie::NativeUpdateAnimation (float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	if (IsValid(ZombieREF))
	{
		bIsGetCharacterMy = ZombieREF->bIsGetCharacterMY;
	}
}
//------------------------------------------------------------------------------------------------------------
