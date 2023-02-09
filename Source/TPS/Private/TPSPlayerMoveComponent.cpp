// Fill out your copyright notice in the Description page of Project Settings.


#include "TPSPlayerMoveComponent.h"
#include "TPSPlayer.h"
#include "GameFramework/CharacterMovementComponent.h"

// Sets default values for this component's properties
UTPSPlayerMoveComponent::UTPSPlayerMoveComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UTPSPlayerMoveComponent::BeginPlay()
{
	Super::BeginPlay();

	me = Cast<ATPSPlayer>(GetOwner());
	
	me->GetCharacterMovement()->MaxWalkSpeed = walkSpeed;
	isCrouch = false;

}


// Called every frame
void UTPSPlayerMoveComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
#pragma region Movement
	FTransform trans(me->GetControlRotation());
	FVector resultDirection = trans.TransformVector(direction);

	resultDirection.Z = 0;
	resultDirection.Normalize();

	me->AddMovementInput(resultDirection);

	direction = FVector::ZeroVector;
#pragma endregion
}

void UTPSPlayerMoveComponent::SetupPlayerInput(UInputComponent* PlayerInputComponent)
{

	PlayerInputComponent->BindAxis(TEXT("Horizontal"), this, &UTPSPlayerMoveComponent::OnAxisHorizontal);
	PlayerInputComponent->BindAxis(TEXT("Vertical"), this, &UTPSPlayerMoveComponent::OnAxisVertical);
	PlayerInputComponent->BindAxis(TEXT("Look Up"), this, &UTPSPlayerMoveComponent::OnAxisLookUp);
	PlayerInputComponent->BindAxis(TEXT("Turn Right"), this, &UTPSPlayerMoveComponent::OnAxisTurnRight);

	PlayerInputComponent->BindAction(TEXT("Jump"), IE_Pressed, this, &UTPSPlayerMoveComponent::OnActionJump);

	PlayerInputComponent->BindAction(TEXT("Run"), IE_Pressed, this, &UTPSPlayerMoveComponent::OnActionRunPressed);
	PlayerInputComponent->BindAction(TEXT("Run"), IE_Released, this, &UTPSPlayerMoveComponent::OnActionRunReleased);

	PlayerInputComponent->BindAction(TEXT("Crouch"), IE_Pressed, this, &UTPSPlayerMoveComponent::OnActionCrouchPressed);

}





void UTPSPlayerMoveComponent::OnAxisHorizontal(float value)
{
	direction.Y = value;
}

void UTPSPlayerMoveComponent::OnAxisVertical(float value)
{
	direction.X = value;
}

void UTPSPlayerMoveComponent::OnAxisLookUp(float value)
{
	//Pitch
	me->AddControllerPitchInput(value);
}

void UTPSPlayerMoveComponent::OnAxisTurnRight(float value)
{
	//Yaw
	me->AddControllerYawInput(value);
}

void UTPSPlayerMoveComponent::OnActionJump()
{
	me->Jump();
}

void UTPSPlayerMoveComponent::OnActionRunPressed()
{
	me->GetCharacterMovement()->MaxWalkSpeed = runSpeed;
}

void UTPSPlayerMoveComponent::OnActionRunReleased()
{
	me->GetCharacterMovement()->MaxWalkSpeed = walkSpeed;
}

void UTPSPlayerMoveComponent::OnActionCrouchPressed()
{
	isCrouch = !isCrouch;
	if (isCrouch) {
		me->GetCharacterMovement()->MaxWalkSpeedCrouched = crouchSpeed;
		me->Crouch();
	}
	else {
		me->GetCharacterMovement()->MaxWalkSpeedCrouched = walkSpeed;
		me->UnCrouch();
	}
}