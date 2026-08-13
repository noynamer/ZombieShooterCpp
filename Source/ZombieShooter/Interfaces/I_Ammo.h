#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "I_Ammo.generated.h"

UINTERFACE(MinimalAPI)
class UI_Ammo : public UInterface
{
	GENERATED_BODY()
};

class ZOMBIESHOOTER_API II_Ammo
{
	GENERATED_BODY()

public:

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void AddAmmo();

};
