// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include <stack>

#include "CoreMinimal.h"

class ICommand;
/**
 * 
 */
class UE5_TICTACTOE_API CommandManager
{
public:
	void ExecuteCommand(TSharedPtr<ICommand> Command);

	void Undo();

	void Redo();

private:
	std::stack<TSharedPtr<ICommand>> UndoStack; // 撤销栈
	std::stack<TSharedPtr<ICommand>> RedoStack; // 重做栈
};
