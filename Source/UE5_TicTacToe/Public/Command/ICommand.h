// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

/**
 * 
 */
class ICommand
{
public:
	ICommand();
	virtual void Execute() = 0;
	virtual void Redo() = 0;
	virtual void Undo() = 0;
	virtual ~ICommand();
};
