#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GameSystem.generated.h"

class ACell;
class AAudioSystem;

UENUM(BlueprintType)
enum class EMark : uint8 {
	None     UMETA(DisplayName = "None"),
	X     UMETA(DisplayName = "X"),
	O     UMETA(DisplayName = "O")
};


DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnStateChanged);

UCLASS()
class UE5_TICTACTOE_API AGameSystem : public AActor
{
	GENERATED_BODY()
	
public:	
	AGameSystem();
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="CPP Settings")
	TObjectPtr<AAudioSystem> AudioSystem;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="CPP Settings")
	bool Turn = true;  //Turn = true: player, Turn = false: AI
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="CPP Settings")
	TArray<TObjectPtr<ACell>> Cells;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="CPP Settings")
	TObjectPtr<ACell> ClickedCell;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="CPP Settings")
	TArray<TObjectPtr<ACell>> WinCells;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="CPP Settings")
	bool IsGameEnd = false;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="CPP Settings")
	int M = 3;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="CPP Settings")
	int N = 3;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="CPP Settings")
	int K = 3;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="CPP Settings")
	EMark Winner = EMark::None;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="CPP Settings")
	int AvailableMoves = 0;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="CPP Settings")
	int WinCountX = 0;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="CPP Settings")
	int WinCountO = 0;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="CPP Settings")
	float CellsOffsetPerc = 10.0f;
	

	//Delegate
	UPROPERTY(BlueprintAssignable, Category = "CPP Settings")
	FOnStateChanged OnGameStart;
	UPROPERTY(BlueprintAssignable, Category = "CPP Settings")
	FOnStateChanged OnGamePause;
	UPROPERTY(BlueprintAssignable, Category = "CPP Settings")
	FOnStateChanged OnGameEnd;
	UPROPERTY(BlueprintAssignable, Category = "CPP Settings")
	FOnStateChanged OnTurnChange;
	
protected:
	virtual void BeginPlay() override;

	

};
