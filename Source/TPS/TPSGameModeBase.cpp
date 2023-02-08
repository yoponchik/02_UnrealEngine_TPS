// Copyright Epic Games, Inc. All Rights Reserved.


#include "TPSGameModeBase.h"
#include "TPS.h"
#include "SpawnManager.h" 
#include "Kismet/GameplayStatics.h"

ATPSGameModeBase::ATPSGameModeBase()
{
	//PRINT_CALL_INFO();

	PRINT_LOG(TEXT("%s %s"), TEXT("ATPSGameModeBase"), TEXT("Error"));
}

void ATPSGameModeBase::BeginPlay()
{
	Super::BeginPlay();

	spawnManager = Cast<ASpawnManager>(UGameplayStatics::GetActorOfClass(GetWorld(), ASpawnManager::StaticClass()));

	LevelUp();
}


//Don't need to do this in Tick, because we have an infinite Timer loop
void ATPSGameModeBase::LevelUp()
{
	level++;													//next level, shouldn;t we do this in Levelup process			
	
	spawnManager->targetSpawnCount = level;						//make the spawnManager's target spawn Count

	spawnManager->spawnCount = 0;						//
	
#pragma region Level Design
	//Method 1
	eXPThreshold = 	level;

	//Method 2
	//eXPThreshold = level * level + 2;							//can do this to make it progressively harder

	//Method 3
	/*if(level < 20){
		eXPThreshold = level;
	}
	else if(level < 40){
		eXPThreshold = level;
	}
	else if(level <60){
		eXPThreshold = level;
	}*/
#pragma endregion Level Design
}

void ATPSGameModeBase::LevelUpProcess()
{
	eXP -= eXPThreshold;						//we don't want to get rid of the remaining experience

	LevelUp();

	if(eXP >= eXPThreshold){
		FTimerHandle timerHandle;
		GetWorldTimerManager().SetTimer(timerHandle, this, &ATPSGameModeBase::LevelUpProcess, 0.2f, false);
	}
}

void ATPSGameModeBase::AddEXP(int amount)
{
	//add experience
	eXP += amount;
	
	//check to see if the exp enough to levelUp; if it reached the EXP Threshold
	if(eXP >= eXPThreshold){
		LevelUpProcess();							//Level
	}
}
