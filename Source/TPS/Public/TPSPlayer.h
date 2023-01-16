// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "TPSPlayer.generated.h"

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

	#pragma region Components
	UPROPERTY(EditAnywhere, Category = "Player Settings | Components")
	class USpringArmComponent* springArmComp;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Player Settings | Gun Settings")
	class UCameraComponent* cameraComp;
	#pragma endregion

	#pragma region Input Actions
	void OnAxisHorizontal(float value);
	void OnAxisVertical(float value);
	void OnAxisLookUp(float value);
	void OnAxisTurnRight(float value);
	
	void OnActionJump();
	void OnActionFirePressed();
	void OnActionFireReleased();
	#pragma endregion
	
#pragma region Movement
	FVector direction;
	float walkSpeed = 600;
#pragma endregion

	void Fire();

	//AutoFire
	FTimerHandle fireTimerHandle;						//this is like a receipt

	UPROPERTY(EditAnywhere, Category = "Player Settings | Gun Settings")
	float fireInterval = 0.5f;

	UPROPERTY(EditAnywhere, Category = "Player Settings | Gun Settings")
	TSubclassOf<class ABulletActor> bulletFactory;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Player Settings | Gun Settings")
	class USkeletalMeshComponent* gunMeshComp;


};
