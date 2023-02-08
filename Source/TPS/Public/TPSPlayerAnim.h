// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "TPSPlayerAnim.generated.h"

/**
 * 
 */
UCLASS()
class TPS_API UTPSPlayerAnim : public UAnimInstance
{
	GENERATED_BODY()

public:

	void NativeUpdateAnimation(float DeltaSeconds) override;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float forwardVelocity;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float rightVelocity;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	bool isInAir;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	bool isCrouching;

	UPROPERTY(EditAnywhere)
	class UAnimMontage* fireMontageFactory;

	UFUNCTION()
	void OnFire();
};
