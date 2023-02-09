// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "TPSPlayerBaseComponent.h"
#include "TPSPlayerGunComponent.generated.h"

#define GRENADELAUNCHER true
#define SNIPER true

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class TPS_API UTPSPlayerGunComponent : public UTPSPlayerBaseComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UTPSPlayerGunComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;


	virtual void SetupPlayerInput(class UInputComponent* PlayerInputComponent) override;

#pragma region Input Actions
	void OnActionFirePressed();
	void OnActionFireReleased();
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

	//cam shake
	UPROPERTY(EditAnywhere)
	TSubclassOf<class UCameraShakeBase> camShakeFactory;

	UPROPERTY(EditAnywhere)
	class UCameraShakeBase* canShakeInstance;


};
