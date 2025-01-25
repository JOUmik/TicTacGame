// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Cell.generated.h"

UCLASS()
class UE5_TICTACTOE_API ACell : public AActor
{
	GENERATED_BODY()
	
public:	
	ACell();
	virtual void Tick(float DeltaTime) override;
	
	
	

	//variables
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "CPP Settings")
	bool IsCellMarked = false;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "CPP Settings")
	bool MarkBool = false;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "CPP Settings")
	int X = 0;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "CPP Settings")
	int Y = 0;

protected:
	virtual void BeginPlay() override;

	

};
