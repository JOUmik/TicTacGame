#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GameSystem.generated.h"

class CommandManager;
class ACell;
class AAudioSystem;

UENUM(BlueprintType)
enum class EMark : uint8 {
	None     UMETA(DisplayName = "None"),
	X     UMETA(DisplayName = "X"),
	O     UMETA(DisplayName = "O")
};

enum class ELine : uint8
{
	Left = 0,
	Top = 1
};

//For AI
struct AIMove
{
	int X;
	int Y;
	int Score;
};


DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnStateChanged);

UCLASS()
class UE5_TICTACTOE_API AGameSystem : public AActor
{
	GENERATED_BODY()
	
public:	
	AGameSystem();
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable)
	void MakeTurn(ACell* i_ClickedCell);
	bool CheckForWin(int X, int Y);

	void AITurn();
	AIMove MiniMaxAlgorithm(int Depth, bool bIsMaximizing);

	void PlaceMarker(int X, int Y, EMark Mark);
	void RemoveMarker(int X, int Y);
	bool CheckWinForMiniMax(EMark Mark);
	bool IsDraw();
	
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void PrepareGame();
	UFUNCTION(BlueprintCallable)
	void RestartGame();

	void ResetGameData();
	
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void HandleBackgroundColor();
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void ChangeWinCellsColor();
	UFUNCTION(BlueprintCallable)
	void ClearAllCells();
	bool IsCellEmpty(int X, int Y);

	ACell* GetCellByIndex(int X, int Y);
	void SetUpCells();
	void SetCellByIndex(const TObjectPtr<ACell>& Cell);
	void SetCellTransform(const TObjectPtr<ACell>& Cell) const;
	void SetCellLines(const TObjectPtr<ACell>& Cell) const;
	FTransform CalcLineTransform(const TObjectPtr<ACell>& Cell, ELine Line) const;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="CPP Settings")
	TObjectPtr<AAudioSystem> AudioSystem;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="CPP Settings")
	bool Turn = true;  //Turn = true: player, Turn = false: AI
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="CPP Settings")
	TArray<ACell*> Cells;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="CPP Settings")
	ACell* ClickedCell;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="CPP Settings")
	TArray<ACell*> WinCells;
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
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="CPP Settings")
	TSubclassOf<ACell> CellClass;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="CPP Settings")
	TSubclassOf<AActor> LineClass;

	UPROPERTY(EditAnywhere, Category="CPP Settings")
	TObjectPtr<UStaticMesh> XMesh;
	UPROPERTY(EditAnywhere, Category="CPP Settings")
	TObjectPtr<UStaticMesh> OMesh;
	
	//Delegate
	UPROPERTY(BlueprintAssignable, BlueprintCallable, Category = "CPP Settings")
	FOnStateChanged OnGameStart;
	UPROPERTY(BlueprintAssignable, BlueprintCallable, Category = "CPP Settings")
	FOnStateChanged OnGamePause;
	UPROPERTY(BlueprintAssignable, BlueprintCallable, Category = "CPP Settings")
	FOnStateChanged OnGameEnd;
	UPROPERTY(BlueprintAssignable, BlueprintCallable, Category = "CPP Settings")
	FOnStateChanged OnTurnChange;
	UPROPERTY(BlueprintAssignable, BlueprintCallable, Category = "CPP Settings")
	FOnStateChanged EnableUndo;
	UPROPERTY(BlueprintAssignable, BlueprintCallable, Category = "CPP Settings")
	FOnStateChanged EnableRedo;
	UPROPERTY(BlueprintAssignable, BlueprintCallable, Category = "CPP Settings")
	FOnStateChanged DisableUndo;
	UPROPERTY(BlueprintAssignable, BlueprintCallable, Category = "CPP Settings")
	FOnStateChanged DisableRedo;

	//undo/redo
	UFUNCTION(BlueprintCallable)
	void UndoMove();

	void ReplaceMarker(int X, int Y, EMark Mark);
	UFUNCTION(BlueprintCallable)
	void RedoMove();
	CommandManager* CommandMgr;
	
protected:
	virtual void BeginPlay() override;

	
private:
	FTimerHandle AITimeHandle;
};
