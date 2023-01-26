// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "TPSPlayer.generated.h"

#define GRENADELAUNCHER true
#define SNIPER true

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

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Player Settings | Gun Settings")
	class USkeletalMeshComponent* gunMeshComp;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Player Settings | Gun Settings")
	class UStaticMeshComponent* sniperMeshComp;
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

	UPROPERTY(EditAnywhere, Category = "Player Settings | Gun Settings")
	TSubclassOf<class ABulletActor> bulletFactory;

	UPROPERTY(EditAnywhere, Category = "Player Settings | Gun Settings")
	float fireInterval = 0.5f;

	//AutoFire
	FTimerHandle fireTimerHandle;						//this is like a receipt

	void Fire();

#pragma region UI Widgets
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player Settings | Gun Settings")
	TSubclassOf<class UUserWidget> crossHairFactory;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player Settings | Gun Settings")
	TSubclassOf<class UUserWidget> sniperCrossHairFactory;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player Settings | Gun Settings")
	class UUserWidget* crossHairUI;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player Settings | Gun Settings")
	class UUserWidget* sniperCrosshairUI;

	void ChooseGun(bool bGrenadeLauncher);

	void OnActionGrenadeLauncher();
	void OnActionSniper();

	void OnActionZoomIn();
	void OnActionZoomOut();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player Settings | Gun Settings")
	bool bIsGrenadeLauncher;
#pragma endregion

	UPROPERTY(EditAnywhere)
	class UParticleSystem* bulletImpactFactory;

	//Animation
	UPROPERTY(EditAnywhere)
	class UAnimMontage* fireMontageFactory;

	UPROPERTY(EditAnywhere)			//don't need UPROPERTY because we find the file in the constructor automatically
	class USoundBase* sniperFireSound;
};
