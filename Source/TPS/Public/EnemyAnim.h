// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "EnemyFSM.h"
#include "EnemyAnim.generated.h"

/**
 * 
 */
UCLASS()
class TPS_API UEnemyAnim : public UAnimInstance
{
	GENERATED_BODY()

public:
	virtual void NativeBeginPlay() override;

	UPROPERTY()
	class AEnemy* me;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	EEnemyState enemyState;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	bool bAttackPlay;

	UFUNCTION()
	void AnimNotify_OnHit();

	UPROPERTY(EditAnywhere, BlueprintReadWrite)	
	bool bEnemyDieEnd;
	
};
