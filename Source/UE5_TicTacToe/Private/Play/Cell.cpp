// Fill out your copyright notice in the Description page of Project Settings.


#include "Play/Cell.h"

#include "Kismet/GameplayStatics.h"
#include "Play/AudioSystem.h"

ACell::ACell()
{
	PrimaryActorTick.bCanEverTick = true;

}

void ACell::BeginPlay()
{
	Super::BeginPlay();

	AudioSystem = Cast<AAudioSystem>(UGameplayStatics::GetActorOfClass(GetWorld(), AAudioSystem::StaticClass()));
	GameSystem = Cast<AGameSystem>(UGameplayStatics::GetActorOfClass(GetWorld(), AGameSystem::StaticClass()));
}

void ACell::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

