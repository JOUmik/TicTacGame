// Fill out your copyright notice in the Description page of Project Settings.


#include "Command/PlaceMarkerCommand.h"

#include "Play/GameSystem.h"

void PlaceMarkerCommand::Execute()
{
}

void PlaceMarkerCommand::Redo()
{
	if (GameSystem)
	{
		GameSystem->ReplaceMarker(X, Y, Mark);
		GameSystem->AvailableMoves--;
	}
}

void PlaceMarkerCommand::Undo()
{
	if (GameSystem)
	{
		GameSystem->RemoveMarker(X, Y);
		GameSystem->AvailableMoves++;
	}
}
