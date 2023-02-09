// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "TPSPlayer.generated.h"

DECLARE_MULTICAST_DELEGATE_OneParam(FSetupPlayerInputDelegate, class UInputComponent*)

UCLASS()
class TPS_API ATPSPlayer : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ATPSPlayer();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;


	UPROPERTY(EditAnywhere, Category = "Player Settings | Components")
	class USpringArmComponent* springArmComp;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Player Settings | Gun Settings")
	class UCameraComponent* cameraComp;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Player Settings | Gun Settings")
	class USkeletalMeshComponent* gunMeshComp;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Player Settings | Gun Settings")
	class UStaticMeshComponent* sniperMeshComp;

	UPROPERTY(EditAnywhere, Category = "Player Settings | Components")
		class UTPSPlayerMoveComponent* moveComp;

	UPROPERTY(EditAnywhere, Category = "Player Settings | Components")
	class UTPSPlayerGunComponent* gunComp;

	FSetupPlayerInputDelegate SetupInputDelegate;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	int hP;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	int maxHP = 2;

	UFUNCTION(BlueprintNativeEvent)
	void OnTakeDamage(int damageAmount);
};
