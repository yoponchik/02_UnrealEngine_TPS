// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy.h"
#include "EnemyFSM.h"
#include "EnemyAnim.h"
// Sets default values
AEnemy::AEnemy()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//Mesh
	ConstructorHelpers::FObjectFinder<USkeletalMesh> tempMesh(TEXT("/Script/Engine.SkeletalMesh'/Game/Characters/Mannequins/Meshes/SKM_Manny.SKM_Manny'"));
	
	if (tempMesh.Succeeded()) {
		GetMesh()->SetSkeletalMesh(tempMesh.Object);
		GetMesh()->SetRelativeLocationAndRotation(FVector(0, 0, -90), FRotator(0, -90, 0));
		GetMesh()->SetRelativeScale3D(FVector(0.85f));
	}

	//Animation
	ConstructorHelpers::FClassFinder<UAnimInstance> tempAnim(TEXT("/Script/Engine.AnimBlueprint'/Game/Enemy/ABP_EnemyAnim.ABP_EnemyAnim_C'"));

	if(tempAnim.Succeeded()){
		GetMesh()->SetAnimInstanceClass(tempAnim.Class);
	}

	//State Machine
	enemyFSM = CreateDefaultSubobject<UEnemyFSM>(TEXT("Enemy FSM"));

}

// Called when the game starts or when spawned
void AEnemy::BeginPlay()
{
	Super::BeginPlay();


	enemyAnim = Cast<UEnemyAnim>(GetMesh()->GetAnimInstance());
}

// Called every frame
void AEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AEnemy::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}



