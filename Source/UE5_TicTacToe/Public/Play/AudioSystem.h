// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameSystem.h"
#include "GameFramework/Actor.h"
#include "AudioSystem.generated.h"

UCLASS()
class UE5_TICTACTOE_API AAudioSystem : public AActor
{
	GENERATED_BODY()
	
public:	
	AAudioSystem();
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void PlayMusic(USoundBase* Sound);
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void PlayWin(EMark Winner);
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void StopWin();
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void PlayMark();
protected:
	virtual void BeginPlay() override;
	

};
