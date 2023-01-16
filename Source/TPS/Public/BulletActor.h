// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BulletActor.generated.h"

UCLASS()
class TPS_API ABulletActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ABulletActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

#pragma region Components
	UPROPERTY(EditAnywhere, Category = "Bullet Settings | Components")
	class USphereComponent* sphereComp;

	UPROPERTY(EditAnywhere, Category = "Bullet Settings | Components")
	class UStaticMeshComponent* meshComp;
	
	UPROPERTY(EditAnywhere, Category = "Bullet Settings | Components")
	class UProjectileMovementComponent* projMoveComp;
#pragma endregion

	UFUNCTION()
	void OnDie();

};
