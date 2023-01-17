// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "BulletActor.h"
#include <Components/SphereComponent.h>
#include <Components/StaticMeshComponent.h>
#include <GameFramework/ProjectileMovementComponent.h>
#include "../TPS.h"


// Sets default values
ABulletActor::ABulletActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	#pragma region Components
	sphereComp = CreateDefaultSubobject<USphereComponent>("Collider Component");
	SetRootComponent(sphereComp);
	sphereComp->SetSphereRadius(13);
	sphereComp->SetCollisionProfileName(TEXT("BlockAll"));

	meshComp= CreateDefaultSubobject<UStaticMeshComponent>("Mesh Component");
	meshComp->SetupAttachment(RootComponent);
	meshComp->SetRelativeScale3D(FVector(0.25f));
	meshComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);					//can do presets
	//meshComp->SetCollisionProfileName(TEXT("NoCollision"));

	projMoveComp = CreateDefaultSubobject<UProjectileMovementComponent>("Projectile Component");
	projMoveComp->SetUpdatedComponent(sphereComp);
	projMoveComp->InitialSpeed = 5000;
	projMoveComp->MaxSpeed = 5000;
	projMoveComp->bShouldBounce = true;						//note that this variable is b because its a bool
	projMoveComp->Bounciness = 0.5f;
	
	#pragma endregion

}

// Called when the game starts or when spawned
void ABulletActor::BeginPlay()
{
	Super::BeginPlay();
	
	//Method 1
	//SetLifeSpan(2);
	
	//Method 2
	//InitialLifeSpan = 2;

	//Method 3
	//FTimerHandle dieTimerHandle;
	//GetWorldTimerManager().SetTimer(dieTimerHandle, this, &ABulletActor::OnDie, 0.1f);

	//Method 4 - Lambda Function
	//Capture(?)
#pragma region Lambda Capture Example
	//auto myPlus = [this](int a, int b)->int {return a + b; };
	//PRINT_LOG(TEXT("%d"), myPlus(10, 20));
#pragma endregion

	FTimerHandle dieTimerHandle;
	GetWorldTimerManager().SetTimer(dieTimerHandle, FTimerDelegate::CreateLambda([this]()->void {this->Destroy(); }), 2, false);
}

// Called every frame
void ABulletActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ABulletActor::OnDie()
{
	Destroy();
}

