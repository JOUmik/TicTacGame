// Fill out your copyright notice in the Description page of Project Settings.


#include "Command/CommandManager.h"

#include "Command/ICommand.h"

void CommandManager::ExecuteCommand(TSharedPtr<ICommand> Command)
{
	Command->Execute();
	UndoStack.push(Command);
	while (!RedoStack.empty())
	{
		RedoStack.pop();
	}
}

void CommandManager::Undo()
{
	if (!UndoStack.empty())
	{
		TSharedPtr<ICommand> Command = UndoStack.top();
		UndoStack.pop();
		Command->Undo();
		RedoStack.push(Command);
	}
	if (!UndoStack.empty())
	{
		TSharedPtr<ICommand> Command = UndoStack.top();
		UndoStack.pop();
		Command->Undo();
		RedoStack.push(Command);
	}
}

void CommandManager::Redo()
{
	if (!RedoStack.empty())
	{
		TSharedPtr<ICommand> Command = RedoStack.top();
		RedoStack.pop();
		Command->Redo();
		UndoStack.push(Command);
	}
	if (!RedoStack.empty())
	{
		TSharedPtr<ICommand> Command = RedoStack.top();
		RedoStack.pop();
		Command->Redo();
		UndoStack.push(Command);
	}
}

