// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Enemy.generated.h"

UCLASS()
class TPS_API AEnemy : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AEnemy();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "FSM")
	class UEnemyFSM* enemyFSM;

	UPROPERTY()
	class UEnemyAnim* enemyAnim;

	//Enemy damage
	//declare and call in c++, implement in blueprint
	UFUNCTION(BlueprintImplementableEvent)
	void OnMyDamage();					//When player damages the enemy
	
};
