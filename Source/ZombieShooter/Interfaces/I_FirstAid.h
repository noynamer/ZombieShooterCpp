#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "I_FirstAid.generated.h"

UINTERFACE(MinimalAPI)
class UI_FirstAid : public UInterface
{
	GENERATED_BODY()
};

class ZOMBIESHOOTER_API II_FirstAid
{
	GENERATED_BODY()

public:

	UFUNCTION(BlueprintNativeEvent)
	void AddHealth();

};
