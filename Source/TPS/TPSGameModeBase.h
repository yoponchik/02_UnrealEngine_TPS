// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "TPSGameModeBase.generated.h"

/**
 * 
 */
UCLASS()
class TPS_API ATPSGameModeBase : public AGameModeBase
{
	GENERATED_BODY()
	
public:
	ATPSGameModeBase();						//making a constructor

	virtual void BeginPlay() override;

	UPROPERTY()
	class ASpawnManager* spawnManager;
	
	//The number of enemies = level #

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int level = 0;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	int eXP =0;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	int eXPThreshold;

	void LevelUp();
	void LevelUpProcess();
	
	void AddEXP(int amount);
};
