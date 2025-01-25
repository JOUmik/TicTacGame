// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AudioSystem.h"
#include "GameFramework/Actor.h"
#include "Cell.generated.h"

class AGameSystem;

UCLASS()
class UE5_TICTACTOE_API ACell : public AActor
{
	GENERATED_BODY()
	
public:	
	ACell();
	virtual void Tick(float DeltaTime) override;
	
	UFUNCTION(BlueprintCallable)
	void AddMark(UStaticMesh* Mesh);
	UFUNCTION(BlueprintCallable)
	void RemoveMark();
	

	//variables
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "CPP Settings")
	bool IsCellMarked = false;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "CPP Settings")
	bool MarkBool = false;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "CPP Settings")
	int X = 0;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "CPP Settings")
	int Y = 0;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "CPP Settings")
	TObjectPtr<AAudioSystem> AudioSystem;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "CPP Settings")
	TObjectPtr<AGameSystem> GameSystem;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "CPP Settings")
	TObjectPtr<UStaticMeshComponent> Mark;

protected:
	virtual void BeginPlay() override;

	

};

inline void ACell::AddMark(UStaticMesh* Mesh)
{
	Mark = NewObject<UStaticMeshComponent>(this);
	Mark->SetStaticMesh(Mesh);

	Mark->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);
	Mark->RegisterComponent();

	IsCellMarked = true;
	if (AudioSystem)
	{
		AudioSystem->PlayMark();
	}
}

inline void ACell::RemoveMark()
{
	if (Mark)
	{
		Mark->DestroyComponent();
		Mark = nullptr;
		IsCellMarked = false;
	}
}