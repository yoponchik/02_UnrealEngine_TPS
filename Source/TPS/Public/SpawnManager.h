// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SpawnManager.generated.h"

UCLASS()
class TPS_API ASpawnManager : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASpawnManager();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	
	UPROPERTY(EditAnywhere)
	float createTime;

	//random spawnTime
	UPROPERTY(EditAnywhere)
	float minTime =1;
	UPROPERTY(EditAnywhere)
	float maxTime =2;
	
	FTimerHandle timerHandleSpawnEnemy;

	UPROPERTY(EditAnywhere)
	TSubclassOf<class AEnemy> enemyFactory;

	void MakeEnemy();
};
