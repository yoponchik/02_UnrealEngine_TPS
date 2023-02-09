// Fill out your copyright notice in the Description page of Project Settings.


#include "TPSPlayerBaseComponent.h"
#include "TPSPlayer.h"

// Sets default values for this component's properties
UTPSPlayerBaseComponent::UTPSPlayerBaseComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
	bWantsInitializeComponent = true;
	// ...
}


// Called when the game starts
void UTPSPlayerBaseComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...


}


// Called every frame
void UTPSPlayerBaseComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UTPSPlayerBaseComponent::InitializeComponent()
{
	me = Cast<ATPSPlayer>(GetOwner());
	if (me) {
		me->SetupInputDelegate.AddUObject(this, &UTPSPlayerBaseComponent::SetupPlayerInput);
	}
	else {
		UE_LOG(LogTemp, Warning, TEXT("UTPSPlayerGunComponent::BeginPlay - Can't find Owner"))
	}
}

