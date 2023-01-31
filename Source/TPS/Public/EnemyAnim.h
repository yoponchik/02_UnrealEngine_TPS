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
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	EEnemyState enemyState;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	bool bAttackPlay;
	
	// //attack animation
	// UFUNCTION(BlueprintCallable)
	// void OnHitEvent();

	UFUNCTION()
	void AnimNotify_OnHit();

	UPROPERTY()
	class AEnemy* me;

	virtual void NativeBeginPlay() override;
};
