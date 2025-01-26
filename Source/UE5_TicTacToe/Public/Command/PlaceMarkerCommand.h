// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ICommand.h"

enum class EMark : uint8;
class AGameSystem;
/**
 * 
 */
class PlaceMarkerCommand : public ICommand
{
public:
	PlaceMarkerCommand(AGameSystem* i_GameSystem, int i_X, int i_Y, EMark i_Mark)
	: X(i_X), Y(i_Y), Mark(i_Mark), GameSystem(i_GameSystem) {}

	virtual void Execute() override;
	virtual void Redo() override;
	virtual void Undo() override;

private:
	int X;
	int Y;
	EMark Mark;
	AGameSystem* GameSystem;
};