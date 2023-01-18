// Fill out your copyright notice in the Description page of Project Settings.


#include "TPSPlayer.h"
#include <GameFramework/SpringArmComponent.h>
#include <Camera/CameraComponent.h>
#include <GameFramework/CharacterMovementComponent.h>
#include "BulletActor.h"
#include <UMG/Public/Blueprint/UserWidget.h>
#include "Blueprint/UserWidget.h"

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
	gunMeshComp->SetupAttachment(GetMesh());
	
	ConstructorHelpers::FObjectFinder<USkeletalMesh> tempGunMesh(TEXT("/Script/Engine.SkeletalMesh'/Game/FPWeapon/Mesh/SK_FPGun.SK_FPGun'"));
	if (tempGunMesh.Succeeded()) {
		gunMeshComp->SetSkeletalMesh(tempGunMesh.Object);
		gunMeshComp->SetRelativeLocationAndRotation(FVector(0, 50, 130), FRotator(0));
	}

	sniperMeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Sniper Mesh Component"));
	sniperMeshComp->SetupAttachment(GetMesh());

	ConstructorHelpers::FObjectFinder<UStaticMesh> tempSniperMesh(TEXT("/Script/Engine.StaticMesh'/Game/Sniper/sniper1.sniper1'"));
	if (tempSniperMesh.Succeeded()) {
		sniperMeshComp->SetStaticMesh(tempSniperMesh.Object);
		sniperMeshComp->SetRelativeLocationAndRotation(FVector(0,50,140), FRotator(0,0,0));
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
}

// Called when the game starts or when spawned
void ATPSPlayer::BeginPlay()
{
	Super::BeginPlay();

	#pragma region Widgets
	crossHairUI = CreateWidget(GetWorld(), crossHairFactory);
	sniperCrosshairUI = CreateWidget(GetWorld(), sniperCrossHairFactory);

	crossHairUI->AddToViewport();
	#pragma endregion

	ChooseGun(true);
	//ChooseGun(GRENADELAUNCHER);

}

// Called every frame
void ATPSPlayer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	#pragma region Movement
	FTransform trans(GetControlRotation());
	FVector resultDirection = trans.TransformVector(direction);

	resultDirection.Z = 0;
	resultDirection.Normalize();

	AddMovementInput(resultDirection);

	direction = FVector::ZeroVector;
	#pragma endregion
}

// Called to bind functionality to input
void ATPSPlayer::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	#pragma region Movement Input
	PlayerInputComponent->BindAxis(TEXT("Horizontal"), this, &ATPSPlayer::OnAxisHorizontal);
	PlayerInputComponent->BindAxis(TEXT("Vertical"), this, &ATPSPlayer::OnAxisVertical);
	PlayerInputComponent->BindAxis(TEXT("Look Up"), this, &ATPSPlayer::OnAxisLookUp);
	PlayerInputComponent->BindAxis(TEXT("Turn Right"), this, &ATPSPlayer::OnAxisTurnRight);
	
	PlayerInputComponent->BindAction(TEXT("Jump"),IE_Pressed, this, &ATPSPlayer::OnActionJump);
	#pragma endregion

#pragma region Gun
	PlayerInputComponent->BindAction(TEXT("Fire"),IE_Pressed, this, &ATPSPlayer::OnActionFirePressed);
	PlayerInputComponent->BindAction(TEXT("Fire"),IE_Released, this, &ATPSPlayer::OnActionFireReleased);


	PlayerInputComponent->BindAction(TEXT("Grenade Launcher"),IE_Pressed, this, &ATPSPlayer::OnActionGrenadeLauncher);
	PlayerInputComponent->BindAction(TEXT("Sniper"),IE_Pressed, this, &ATPSPlayer::OnActionSniper);
	
	PlayerInputComponent->BindAction(TEXT("Zoom"),IE_Pressed, this, &ATPSPlayer::OnActionZoomIn);
	PlayerInputComponent->BindAction(TEXT("Zoom"),IE_Released, this, &ATPSPlayer::OnActionZoomOut);
#pragma endregion
}

#pragma region Input Actions
void ATPSPlayer::OnAxisHorizontal(float value)
{
	direction.Y = value;
}

void ATPSPlayer::OnAxisVertical(float value)
{
	direction.X = value;
}

void ATPSPlayer::OnAxisLookUp(float value)
{
	//Pitch
	AddControllerPitchInput(value);
}

void ATPSPlayer::OnAxisTurnRight(float value)
{
	//Yaw
	AddControllerYawInput(value);
}

void ATPSPlayer::OnActionJump()
{
	Jump();
}

void ATPSPlayer::OnActionFirePressed()
{
	//Fire();

	//AutoFire
	GetWorldTimerManager().SetTimer(fireTimerHandle, this, &ATPSPlayer::Fire, fireInterval, true);

	Fire();
}

void ATPSPlayer::OnActionFireReleased()
{
	GetWorldTimerManager().ClearTimer(fireTimerHandle);

}
#pragma endregion

void ATPSPlayer::Fire()
{
	FTransform firePosition = gunMeshComp->GetSocketTransform(TEXT("FirePosition"));

	firePosition.SetRotation(FQuat(GetControlRotation()));

	GetWorld()->SpawnActor<ABulletActor>(bulletFactory, firePosition);
}

void ATPSPlayer::ChooseGun(bool bGrenadeLauncher)
{

	//If current gun is a sniper, and I want to change to the grenade launcher
	if (!bIsGrenadeLauncher && bGrenadeLauncher) {
		cameraComp->SetFieldOfView(90);
		crossHairUI->AddToViewport();
		sniperCrosshairUI->RemoveFromParent();
	}

	bIsGrenadeLauncher = bGrenadeLauncher;

	gunMeshComp->SetVisibility(bGrenadeLauncher);
	sniperMeshComp->SetVisibility(!bGrenadeLauncher);
	
	//Same thing
	//sniperMeshComp->SetVisibility(bChooseGrenadeLauncher ? false : true);
}

void ATPSPlayer::OnActionGrenadeLauncher()
{
	ChooseGun(true);
	//Same using macros
	//ChooseGun(GRENADELAUNCHER);
}

void ATPSPlayer::OnActionSniper()
{
	ChooseGun(false);

	//Same using macros
	//ChooseGun(SNIPER);
}

void ATPSPlayer::OnActionZoomIn()
{
	if (bIsGrenadeLauncher) { return; }

	cameraComp->SetFieldOfView(30);

	crossHairUI->RemoveFromParent();
	sniperCrosshairUI->AddToViewport();
}

void ATPSPlayer::OnActionZoomOut()
{
	if (bIsGrenadeLauncher) { return; }

	cameraComp->SetFieldOfView(90);

	sniperCrosshairUI->RemoveFromParent();
	crossHairUI->AddToViewport();
}

