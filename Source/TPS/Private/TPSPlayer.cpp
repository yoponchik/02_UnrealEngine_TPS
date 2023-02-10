// Fill out your copyright notice in the Description page of Project Settings.


#include "TPSPlayer.h"
#include <GameFramework/SpringArmComponent.h>
#include <Camera/CameraComponent.h>
#include <GameFramework/CharacterMovementComponent.h>
#include "TPSPlayerMoveComponent.h"
#include "TPSPlayerGunComponent.h"

// Sets default values
ATPSPlayer::ATPSPlayer()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	#pragma region Setup Components
	//1. read the mesh of the asset
	//making a tempMesh variable and putting in the 3D model
	ConstructorHelpers::FObjectFinder<USkeletalMesh> tempMesh(TEXT("/Script/Engine.SkeletalMesh'/Game/Characters/Mannequins/Meshes/SKM_Quinn.SKM_Quinn'"));
	// 2. if succeeded.
	if (tempMesh.Succeeded()) {
	//if(tempMesh.Object !=nullptr){					//same thing
		// 3.apply to skeletal mesh in the mesh component(?)
		GetMesh()->SetSkeletalMesh(tempMesh.Object);
		// 4. edit the transform
		GetMesh()->SetRelativeLocationAndRotation(FVector(0,0,-90), FRotator(0,-90,0));
	}

	springArmComp = CreateDefaultSubobject<USpringArmComponent>(TEXT("Spring Arm Component"));
	cameraComp = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera Component"));
	
	//regular gun component
	gunMeshComp = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Gun Skeletal Component"));
	gunMeshComp->SetupAttachment(GetMesh(), TEXT("hand_rSocket"));

	
	ConstructorHelpers::FObjectFinder<USkeletalMesh> tempGunMesh(TEXT("/Script/Engine.SkeletalMesh'/Game/FPWeapon/Mesh/SK_FPGun.SK_FPGun'"));
	if (tempGunMesh.Succeeded()) {
		gunMeshComp->SetSkeletalMesh(tempGunMesh.Object);
		gunMeshComp->SetRelativeLocationAndRotation(FVector((-7.210181,4.476700,-1.526383)), FRotator((9.846552,100.151081,-6.164327)));
	}

	sniperMeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Sniper Mesh Component"));
	sniperMeshComp->SetupAttachment(GetMesh(),TEXT("hand_rSocket"));

	ConstructorHelpers::FObjectFinder<UStaticMesh> tempSniperMesh(TEXT("/Script/Engine.StaticMesh'/Game/Sniper/sniper1.sniper1'"));
	if (tempSniperMesh.Succeeded()) {
		sniperMeshComp->SetStaticMesh(tempSniperMesh.Object);
		sniperMeshComp->SetRelativeLocationAndRotation(FVector((-39.736726,-1.088210,5.370206)), FRotator(13.476263,93.889126,-5.453882));
		sniperMeshComp->SetRelativeScale3D(FVector(0.15f));
	}
#pragma endregion

	#pragma region Components Default Values
	springArmComp->SetupAttachment(RootComponent);
	cameraComp->SetupAttachment(springArmComp);

	//Setup default values for the spring arm
	springArmComp->SetRelativeLocation(FVector(0, 50, 100));
	springArmComp->TargetArmLength = 250;

	bUseControllerRotationYaw = true;
	springArmComp->bUsePawnControlRotation = true;
	cameraComp->bUsePawnControlRotation = true;

	GetCharacterMovement()->bOrientRotationToMovement = true;

	//same thing; might be easier to add include this way?
	//UCharacterMovementComponent* cMC = GetCharacterMovement();
	//cMC->bOrientRotationToMovement = true;
#pragma endregion

	moveComp = CreateDefaultSubobject<UTPSPlayerMoveComponent>(TEXT("Move Component"));
	gunComp = CreateDefaultSubobject<UTPSPlayerGunComponent>(TEXT("Gun Component"));
}

// Called when the game starts or when spawned
void ATPSPlayer::BeginPlay()
{
	Super::BeginPlay();

	hP = maxHP;

}

// Called every frame
void ATPSPlayer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ATPSPlayer::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	SetupInputDelegate.Broadcast(PlayerInputComponent);

}

void ATPSPlayer::OnTakeDamage_Implementation(int damageAmount)
{
	hP -= damageAmount;

	//if player health is less than or equal to zero
	if (hP <= 0) {
		//show gameover screen
		OnGameOver();
	}
}
