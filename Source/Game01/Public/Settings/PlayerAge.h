
#include "CoreMinimal.h"
#include "PlayerAge.generated.h"

USTRUCT(BlueprintType)
struct FPlayerAge
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere)
	int AgeId;
	UPROPERTY(EditAnywhere)
	float CapsuleHeight;
	UPROPERTY(EditAnywhere)
	float CapsuleRadius;
	UPROPERTY(EditAnywhere)
	bool bCanShoot;
	UPROPERTY(EditAnywhere)
	bool bCanDash;
};

