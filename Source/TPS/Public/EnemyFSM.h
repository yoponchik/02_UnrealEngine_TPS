// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "EnemyFSM.generated.h"


UENUM(BlueprintType)
enum class EEnemyState : uint8 {
	IDLE, 
	MOVE,
	ATTACK, 
	DAMAGE, 
	DIE
};

UENUM(BlueprintType)
enum class EEnemyMoveSubState : uint8
{
	PATROL,
	CHASE,
	OLD_MOVE
};

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class TPS_API UEnemyFSM : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UEnemyFSM();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	EEnemyState enemyState;
	void SetState(EEnemyState next);

	EEnemyMoveSubState moveSubState;
	
	
	UPROPERTY()
	class ATPSPlayer* target;

	UPROPERTY()
	class AEnemy* me;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "FSM")
	float attackDistance = 200;

	float currentTime = 0;
	bool bAttackPlay;

	void OnDamageProcess(int damageAmount);



	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "FSM")
	float attackDelayTime = 3;	

	//animation
	void OnHitEvent();

	//AI Movement
	class AAIController* aI;

	bool UpdateRandomLocation(float radius, FVector& outLocation);

	UPROPERTY(EditAnywhere)
	float randomLocationRadius = 500;

	FVector randomLocation;

	//Patrol
	UPROPERTY()
	class APathManager* pathManager;

	int wayPtIndex;
private:
	void OnTickIdle();
	void OnTickDie();
	void OnTickDamage();
	void OnTickAttack();
	void OnTickMove();

	void OnTickOldMove();
	void OnTickPatrol();
	void OnTickChase();


};
