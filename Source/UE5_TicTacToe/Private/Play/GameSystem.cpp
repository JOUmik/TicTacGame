#include "Play/GameSystem.h"

#include "Quaternion.h"
#include "Kismet/GameplayStatics.h"
#include "Play/Cell.h"


AGameSystem::AGameSystem()
{
	PrimaryActorTick.bCanEverTick = true;

}

void AGameSystem::BeginPlay()
{
	Super::BeginPlay();
	Cells.SetNum(9);
}

void AGameSystem::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AGameSystem::MakeTurn(ACell* i_ClickedCell)
{
	ClickedCell = i_ClickedCell;
	if (!IsGameEnd)
	{
		//Made by player(X)
		if (Turn)
		{
			ClickedCell->AddMark(XMesh);
			ClickedCell->MarkBool = true;
		}
		//Made by AI(O)
		else
		{
			ClickedCell->AddMark(OMesh);
			ClickedCell->MarkBool = false;
		}

		Turn = !Turn;
		AvailableMoves--;
		if (CheckForWin(ClickedCell->X, ClickedCell->Y))
		{
			IsGameEnd = true;
			if (ClickedCell->MarkBool)
			{
				Winner = EMark::X;
			}
			else
			{
				Winner = EMark::O;
			}
		}

		HandleBackgroundColor();
		OnTurnChange.Broadcast();

		if (IsGameEnd)
		{
			if (Winner == EMark::X)
			{
				WinCountX++;
			}
			else
			{
				WinCountO++;
			}
			ChangeWinCellsColor();
			AudioSystem->PlayWin(Winner);
			OnGameEnd.Broadcast();
		}
		else if (AvailableMoves == 0)
		{
			Winner = EMark::None;
			IsGameEnd = true;
			OnTurnChange.Broadcast();
			AudioSystem->PlayWin(Winner);
			OnGameEnd.Broadcast();
		}
		else if (!Turn)
		{
			GetWorldTimerManager().SetTimer(AITimeHandle, this, &AGameSystem::AITurn, FMath::RandRange(0.6f, 1.5f));
		}
	}
}

bool AGameSystem::CheckForWin(int X, int Y)
{
	//diagonal
	if (X == Y)
	{
		ACell* A = GetCellByIndex(1, 1);
		ACell* B = GetCellByIndex(2, 2);
		ACell* C = GetCellByIndex(3, 3);
		if (A->IsCellMarked && B->IsCellMarked && C->IsCellMarked)
		{
			if (A->MarkBool == B->MarkBool && B->MarkBool == C->MarkBool)
			{
				WinCells.Add(A);
				WinCells.Add(B);
				WinCells.Add(C);
				return true;
			}
		}
	}
	//anti diagonal
	if (X + Y == 4)
	{
		ACell* A = GetCellByIndex(1, 3);
		ACell* B = GetCellByIndex(2, 2);
		ACell* C = GetCellByIndex(3, 1);
		if (A->IsCellMarked && B->IsCellMarked && C->IsCellMarked)
		{
			if (A->MarkBool == B->MarkBool && B->MarkBool == C->MarkBool)
			{
				WinCells.Add(A);
				WinCells.Add(B);
				WinCells.Add(C);
				return true;
			}
		}
	}
	//X axis
	ACell* A = GetCellByIndex(1, Y);
	ACell* B = GetCellByIndex(2, Y);
	ACell* C = GetCellByIndex(3, Y);
	if (A->IsCellMarked && B->IsCellMarked && C->IsCellMarked)
	{
		if (A->MarkBool == B->MarkBool && B->MarkBool == C->MarkBool)
		{
			WinCells.Add(A);
			WinCells.Add(B);
			WinCells.Add(C);
			return true;
		}
	}
	//Y axis
	A = GetCellByIndex(X, 1);
	B = GetCellByIndex(X, 2);
	C = GetCellByIndex(X, 3);
	if (A->IsCellMarked && B->IsCellMarked && C->IsCellMarked)
	{
		if (A->MarkBool == B->MarkBool && B->MarkBool == C->MarkBool)
		{
			WinCells.Add(A);
			WinCells.Add(B);
			WinCells.Add(C);
			return true;
		}
	}

	return false;
}

void AGameSystem::AITurn()
{
	AIMove BestMove = MiniMaxAlgorithm(0, true);
	
	if (BestMove.X != -1 && BestMove.Y != -1)
	{
		MakeTurn(GetCellByIndex(BestMove.X, BestMove.Y));
	}
}

AIMove AGameSystem::MiniMaxAlgorithm(int Depth, bool bIsMaximizing)
{
	if (Depth >= 4)
	{
		return { -1, -1, 0 };
	}
	
	// detect termination conditions
	if (CheckWinForMiniMax(EMark::O))
	{
		return { -1, -1, 10 - Depth };
	}
	if (CheckWinForMiniMax(EMark::X))
	{
		return { -1, -1, Depth - 10 };
	}
	if (IsDraw())
	{
		return { -1, -1, 0 };
	}

	// Minimax Logic
	AIMove BestMove = { -1, -1, bIsMaximizing ? -1000 : 1000 };

	for (int x = 1; x <= M; ++x)
	{
		for (int y = 1; y <= N; ++y)
		{
			if (IsCellEmpty(x, y))
			{
				// simulate chessing
				PlaceMarker(x, y, bIsMaximizing ? EMark::O : EMark::X);

				// 递归计算得分
				AIMove SimulatedMove = MiniMaxAlgorithm(Depth + 1, !bIsMaximizing);

				// 撤销下棋
				RemoveMarker(x, y);

				// update best score
				SimulatedMove.X = x;
				SimulatedMove.Y = y;

				if (bIsMaximizing)
				{
					if (SimulatedMove.Score > BestMove.Score)
					{
						BestMove = SimulatedMove;
					}
				}
				else
				{
					if (SimulatedMove.Score < BestMove.Score)
					{
						BestMove = SimulatedMove;
					}
				}
			}
		}
	}

	return BestMove;
}

void AGameSystem::PlaceMarker(int X, int Y, EMark Mark)
{
	ACell* A = GetCellByIndex(X, Y);
	A->IsCellMarked = true;
	if (Mark == EMark::O)
	{
		A->MarkBool = false;
	}
	else
	{
		A->MarkBool = true;
	}
}

void AGameSystem::RemoveMarker(int X, int Y)
{
	ACell* A = GetCellByIndex(X, Y);
	A->IsCellMarked = false;
	A->MarkBool = false;
}

bool AGameSystem::CheckWinForMiniMax(EMark Mark)
{
	//Player
	if (Mark == EMark::X)
	{
		for (int i = 1; i<=M; i++)
		{
			ACell* A = GetCellByIndex(i, 1);
			ACell* B = GetCellByIndex(i, 2);
			ACell* C = GetCellByIndex(i, 3);
			if (A->IsCellMarked && B->IsCellMarked && C->IsCellMarked)
			{
				if (A->MarkBool == true && A->MarkBool == B->MarkBool && B->MarkBool == C->MarkBool)
				{
					return true;
				}
			}
		}
		for (int i = 1; i<=N; i++)
		{
			ACell* A = GetCellByIndex(1, i);
			ACell* B = GetCellByIndex(2, i);
			ACell* C = GetCellByIndex(3, i);
			if (A->IsCellMarked && B->IsCellMarked && C->IsCellMarked)
			{
				if (A->MarkBool == true && A->MarkBool == B->MarkBool && B->MarkBool == C->MarkBool)
				{
					return true;
				}
			}
		}
		ACell* A = GetCellByIndex(1, 1);
		ACell* B = GetCellByIndex(2, 2);
		ACell* C = GetCellByIndex(3, 3);
		if (A->IsCellMarked && B->IsCellMarked && C->IsCellMarked)
		{
			if (A->MarkBool == true && A->MarkBool == B->MarkBool && B->MarkBool == C->MarkBool)
			{
				return true;
			}
		}

		A = GetCellByIndex(1, 3);
		B = GetCellByIndex(2, 2);
		C = GetCellByIndex(3, 1);
		if (A->IsCellMarked && B->IsCellMarked && C->IsCellMarked)
		{
			if (A->MarkBool == true && A->MarkBool == B->MarkBool && B->MarkBool == C->MarkBool)
			{
				return true;
			}
		}

		return false;
	}
	//AI
	else
	{
		for (int i = 1; i<=M; i++)
		{
			ACell* A = GetCellByIndex(i, 1);
			ACell* B = GetCellByIndex(i, 2);
			ACell* C = GetCellByIndex(i, 3);
			if (A->IsCellMarked && B->IsCellMarked && C->IsCellMarked)
			{
				if (A->MarkBool == false && A->MarkBool == B->MarkBool && B->MarkBool == C->MarkBool)
				{
					return true;
				}
			}
		}
		for (int i = 1; i<=N; i++)
		{
			ACell* A = GetCellByIndex(1, i);
			ACell* B = GetCellByIndex(2, i);
			ACell* C = GetCellByIndex(3, i);
			if (A->IsCellMarked && B->IsCellMarked && C->IsCellMarked)
			{
				if (A->MarkBool == false && A->MarkBool == B->MarkBool && B->MarkBool == C->MarkBool)
				{
					return true;
				}
			}
		}
		ACell* A = GetCellByIndex(1, 1);
		ACell* B = GetCellByIndex(2, 2);
		ACell* C = GetCellByIndex(3, 3);
		if (A->IsCellMarked && B->IsCellMarked && C->IsCellMarked)
		{
			if (A->MarkBool == false && A->MarkBool == B->MarkBool && B->MarkBool == C->MarkBool)
			{
				return true;
			}
		}

		A = GetCellByIndex(1, 3);
		B = GetCellByIndex(2, 2);
		C = GetCellByIndex(3, 1);
		if (A->IsCellMarked && B->IsCellMarked && C->IsCellMarked)
		{
			if (A->MarkBool == false && A->MarkBool == B->MarkBool && B->MarkBool == C->MarkBool)
			{
				return true;
			}
		}

		return false;
	}
}

bool AGameSystem::IsDraw()
{
	for (auto Cell:Cells)
	{
		if (!Cell->IsCellMarked)
		{
			return false;
		}
	}
	return true;
}

void AGameSystem::RestartGame()
{
	ClearAllCells();
	ResetGameData();
	AudioSystem->StopWin();
}

void AGameSystem::ResetGameData()
{
	IsGameEnd = false;
	Winner = EMark::None;
	Turn = true;
	OnTurnChange.Broadcast();
	AvailableMoves = Cells.Num();
	WinCells.Empty();
	HandleBackgroundColor();
}

void AGameSystem::PrepareGame_Implementation()
{
	SetUpCells();

	AudioSystem = Cast<AAudioSystem>(UGameplayStatics::GetActorOfClass(GetWorld(), AAudioSystem::StaticClass()));

	HandleBackgroundColor();
}

void AGameSystem::ClearAllCells()
{
	for(auto Cell : Cells)
	{
		Cell->ClearCell();
	}
}

bool AGameSystem::IsCellEmpty(int X, int Y)
{
	if (!GetCellByIndex(X, Y)->IsCellMarked)
	{
		return true;
	}
	return false;
}

ACell* AGameSystem::GetCellByIndex(int X, int Y)
{
	return Cells[(X-1)*N+(Y-1)];
}

void AGameSystem::SetUpCells()
{
	for (int i = 1; i<=M; i++)
	{
		for (int j = 1; j<=N; j++)
		{
			TObjectPtr<ACell> Cell = GetWorld()->SpawnActor<ACell>(CellClass);
			Cell->X = i;
			Cell->Y = j;
			SetCellByIndex(Cell);
			SetCellTransform(Cell);
			SetCellLines(Cell);
		}
	}
	AvailableMoves = Cells.Num();
}

void AGameSystem::SetCellByIndex(const TObjectPtr<ACell>& Cell)
{
	Cells[(Cell->X-1)*N+(Cell->Y-1)] = Cell;
}

void AGameSystem::SetCellTransform(const TObjectPtr<ACell>& Cell) const
{
	FVector Origin;
	FVector Extent;
	Cell->GetActorBounds(false, Origin, Extent);
	float LocationX = 0;
	float LocationY = 0;

	float CellLineX = Extent.X/100.0f*CellsOffsetPerc*2;
	float CellLineY = Extent.Y/100.0f*CellsOffsetPerc*2;
	float CellSizeX = Extent.X * 2.0f * (Cell->Y - 1);
	float CellSizeY = Extent.Y * 2.0f * (Cell->X - 1);
	float CellX = CellSizeX + CellLineX * (Cell->Y - 1);
	float CellY = CellSizeY + CellLineY * (Cell->X - 1);
	Cell->SetActorLocation(FVector(CellX, CellY, 0));
}

void AGameSystem::SetCellLines(const TObjectPtr<ACell>& Cell) const
{
	if (Cell->X > 1)
	{
		AActor* actor = GetWorld()->SpawnActor<AActor>(LineClass);
		actor->SetActorTransform(CalcLineTransform(Cell, ELine::Left));
	}
	if (Cell->Y > 1)
	{
		AActor* actor = GetWorld()->SpawnActor<AActor>(LineClass);
		actor->SetActorTransform(CalcLineTransform(Cell, ELine::Top));
	}
}

FTransform AGameSystem::CalcLineTransform(const TObjectPtr<ACell>& Cell, const ELine Line) const
{
	FTransform LineTransform;
	FTransform CellTransform = Cell->GetActorTransform();
	FVector Origin;
	FVector Extent;
	Cell->GetActorBounds(false, Origin, Extent);
	if (Line == ELine::Left)
	{
		float LocationX = CellTransform.GetLocation().X;
		float LocationY = CellTransform.GetLocation().Y - Extent.Y - Extent.Y / 100.f * CellsOffsetPerc;
		float LocationZ = CellTransform.GetLocation().Z;
		float Roll = CellTransform.GetRotation().X;
		float Pitch = CellTransform.GetRotation().Y;
		float Yaw = CellTransform.GetRotation().Z + 90.f;
		float ScaleX = CellTransform.GetScale3D().X / 100.f * CellsOffsetPerc;
		float ScaleY = CellTransform.GetScale3D().Y / 100.f * (CellsOffsetPerc + 100.f);
		float ScaleZ = CellTransform.GetScale3D().Z / 100.f * CellsOffsetPerc;
		LineTransform.SetLocation(FVector(LocationX, LocationY, LocationZ));
		FRotator Rotator = FRotator(Pitch, Yaw, Roll);
		LineTransform.SetRotation(Rotator.Quaternion());
		LineTransform.SetScale3D(FVector(ScaleX, ScaleY, ScaleZ));
	}
	else if (Line == ELine::Top)
	{
		float LocationX = CellTransform.GetLocation().X - Extent.X - Extent.X /100.f * CellsOffsetPerc;
		float LocationY = CellTransform.GetLocation().Y;
		float LocationZ = CellTransform.GetLocation().Z;
		float Roll = CellTransform.GetRotation().X;
		float Pitch = CellTransform.GetRotation().Y;
		float Yaw = CellTransform.GetRotation().Z;
		float ScaleX = CellTransform.GetScale3D().X / 100.f * CellsOffsetPerc;
		float ScaleY = CellTransform.GetScale3D().Y / 100.f * (CellsOffsetPerc + 100.f);
		float ScaleZ = CellTransform.GetScale3D().Z / 100.f * CellsOffsetPerc;
		LineTransform.SetLocation(FVector(LocationX, LocationY, LocationZ));
		FRotator Rotator = FRotator(Pitch, Yaw, Roll);
		LineTransform.SetRotation(Rotator.Quaternion());
		LineTransform.SetScale3D(FVector(ScaleX, ScaleY, ScaleZ));
	}
	
	return LineTransform;
}
