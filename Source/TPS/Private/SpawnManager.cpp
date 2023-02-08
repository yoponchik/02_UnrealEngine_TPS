// Fill out your copyright notice in the Description page of Project Settings.


#include "SpawnManager.h"
#include "Enemy.h"
#include "SpawnPoint.h"
#include "Kismet/GameplayStatics.h"
#include "TPS/TPSGameModeBase.h"

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
	//if there are still spawnCounts to be made
	if(spawnCount < targetSpawnCount){
		
		spawnCount++;										//keep count of enemies spawned
		
		int randomIndex = 0;								//initialize 
		randomIndex = FMath::RandRange(0, spawnList.Num() -1);

		if(randomIndex != prevRandIndex){
			randomIndex = (randomIndex + 1) % spawnList.Num();
		}
		prevRandIndex = randomIndex;
	
		FVector spawnLocation = spawnList[randomIndex]->GetActorLocation();
		FRotator spawnRotation = spawnList[randomIndex]->GetActorRotation();

#pragma region Spawn Enemies
		//Method 1
		//GetWorld()->SpawnActor<AEnemy>(enemyFactory, spawnLocation, spawnRotation);

		//Method 2
		//get a random index number; either 0 or 1
		/*
		int randomSpawnIndex = FMath::RandRange(0,enemyFactory.Num() - 1);
		//put random index number
		GetWorld()->SpawnActor<AEnemy>(enemyFactory[randomSpawnIndex], spawnLocation, spawnRotation);
		*/

		//Method 3
		int randomSpawnIndex = FMath::RandRange(0,99);
		int spawnProbability = 50;

		int level = Cast<ATPSGameModeBase>(GetWorld()->GetAuthGameMode())->level;

		if(level < 3){						//if spawn probability is < 3, make the spawn probability 50
			spawnProbability = 50;
		}
		else{								//if level 3 is completed, make spawn probability 20
			spawnProbability = 20;
		}
		
		if(randomSpawnIndex < spawnProbability){			//if spawn Probability is 20, 20 percent of easy enemies made
			GetWorld()->SpawnActor<AEnemy>(enemyFactory[0], spawnLocation, spawnRotation);
		}
		else if(randomSpawnIndex > spawnProbability){		//if spawn probability is 20, 80 percent of hard enemies made
			GetWorld()->SpawnActor<AEnemy>(enemyFactory[1], spawnLocation, spawnRotation);
		}
#pragma endregion Spawn Enemies
	}
	
	int randTime = FMath::RandRange(minTime, maxTime);

	GetWorldTimerManager().SetTimer(timerHandleSpawnEnemy, this, &ASpawnManager::MakeEnemy, 2);
}

