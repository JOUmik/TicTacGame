#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GameSystem.generated.h"

UENUM(BlueprintType)
enum class EMark : uint8 {
	None     UMETA(DisplayName = "None"),
	X     UMETA(DisplayName = "X"),
	O     UMETA(DisplayName = "O")
};


UCLASS()
class UE5_TICTACTOE_API AGameSystem : public AActor
{
	GENERATED_BODY()
	
public:	
	AGameSystem();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

};
