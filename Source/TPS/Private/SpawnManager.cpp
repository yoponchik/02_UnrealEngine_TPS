// Fill out your copyright notice in the Description page of Project Settings.


#include "SpawnManager.h"
#include "Enemy.h"
#include "SpawnPoint.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
ASpawnManager::ASpawnManager()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ASpawnManager::BeginPlay()
{
	Super::BeginPlay();

	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ASpawnPoint::StaticClass(), spawnList);
	
	int randTime = FMath::RandRange(minTime, maxTime);
	GetWorldTimerManager().SetTimer(timerHandleSpawnEnemy, this, &ASpawnManager::MakeEnemy, 2);

}

// Called every frame
void ASpawnManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	//Spawn enemies at my position at a certain interval
}

void ASpawnManager::MakeEnemy()
{

	int randomIndex = 0;

	randomIndex = FMath::RandRange(0, spawnList.Num() -1);

	if(randomIndex != prevRandIndex){
		randomIndex = (randomIndex + 1) % spawnList.Num();
	}
	prevRandIndex = randomIndex;
	
	FVector spawnLocation = spawnList[randomIndex]->GetActorLocation();
	FRotator spawnRotation = spawnList[randomIndex]->GetActorRotation();
	GetWorld()->SpawnActor<AEnemy>(enemyFactory, spawnLocation, spawnRotation);
	
	int randTime = FMath::RandRange(minTime, maxTime);

	GetWorldTimerManager().SetTimer(timerHandleSpawnEnemy, this, &ASpawnManager::MakeEnemy, 2);

}

