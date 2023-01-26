// Fill out your copyright notice in the Description page of Project Settings.


#include "TPSPlayerAnim.h"

#include "TPSPlayer.h"
#include "GameFramework/CharacterMovementComponent.h"

void UTPSPlayerAnim::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	//get owner's velocity
	ATPSPlayer* owner =Cast<ATPSPlayer>(TryGetPawnOwner());
	
	if(!owner){return;}
	
	//split into rightVelocity and forwardVelocity
	FVector velocity = owner->GetVelocity();
	forwardVelocity = FVector::DotProduct(velocity, owner->GetActorForwardVector());
	rightVelocity = FVector::DotProduct(velocity, owner->GetActorRightVector());

	//get isInAir
	isInAir = owner->GetCharacterMovement()->IsFalling();

	//same thing
	/*UCharacterMovementComponent* moveComp;
	moveComp = owner->GetCharacterMovement();*/
	
}

void UTPSPlayerAnim::OnFire()
{
	Montage_Play(fireMontageFactory);
}
