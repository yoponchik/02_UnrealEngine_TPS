// Fill out your copyright notice in the Description page of Project Settings.


#include "TPSPlayerGunComponent.h"

#include "BulletActor.h"
#include <UMG/Public/Blueprint/UserWidget.h>
#include "Blueprint/UserWidget.h"
#include <Kismet/GameplayStatics.h>
#include "Enemy.h"
#include "EnemyFSM.h"
#include "TPSPlayerAnim.h"
#include "TPSPlayer.h"

#include <Camera/CameraComponent.h>


// Sets default values for this component's properties
UTPSPlayerGunComponent::UTPSPlayerGunComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	ConstructorHelpers::FObjectFinder<USoundBase> tempFireSound(TEXT("/Script/Engine.SoundWave'/Game/Sniper/Rifle.Rifle'"));

	if (tempFireSound.Succeeded()) {
		sniperFireSound = tempFireSound.Object;
	}



}


// Called when the game starts
void UTPSPlayerGunComponent::BeginPlay()
{
	Super::BeginPlay();

	me = Cast<ATPSPlayer>(GetOwner());



#pragma region Widgets
	crossHairUI = CreateWidget(GetWorld(), crossHairFactory);
	sniperCrosshairUI = CreateWidget(GetWorld(), sniperCrossHairFactory);

	crossHairUI->AddToViewport();
#pragma endregion

	ChooseGun(true);
	//ChooseGun(GRENADELAUNCHER);
}


// Called every frame
void UTPSPlayerGunComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}


void UTPSPlayerGunComponent::SetupPlayerInput(UInputComponent* PlayerInputComponent)
{
	PlayerInputComponent->BindAction(TEXT("Fire"), IE_Pressed, this, &UTPSPlayerGunComponent::OnActionFirePressed);
	PlayerInputComponent->BindAction(TEXT("Fire"), IE_Released, this, &UTPSPlayerGunComponent::OnActionFireReleased);


	PlayerInputComponent->BindAction(TEXT("Grenade Launcher"), IE_Pressed, this, &UTPSPlayerGunComponent::OnActionGrenadeLauncher);
	PlayerInputComponent->BindAction(TEXT("Sniper"), IE_Pressed, this, &UTPSPlayerGunComponent::OnActionSniper);

	PlayerInputComponent->BindAction(TEXT("Zoom"), IE_Pressed, this, &UTPSPlayerGunComponent::OnActionZoomIn);
	PlayerInputComponent->BindAction(TEXT("Zoom"), IE_Released, this, &UTPSPlayerGunComponent::OnActionZoomOut);
}


void UTPSPlayerGunComponent::OnActionFirePressed()
{

#pragma region Camera Shake
	APlayerCameraManager* cameraManager = UGameplayStatics::GetPlayerCameraManager(GetWorld(), 0);

	//if already shaking cancel, and shake again
	//check if canShakeInstance is nullptr
	if (canShakeInstance && !canShakeInstance->IsFinished()) {
		cameraManager->StopCameraShake(canShakeInstance);
	}

	canShakeInstance = cameraManager->StartCameraShake((camShakeFactory));

	//Same thing
	/*auto controller  = Cast<APlayerController>(GetController());
	auto cameraManager = controller->PlayerCameraManager;*/
#pragma endregion 

	//Play shooting animation
	UTPSPlayerAnim* anim = Cast<UTPSPlayerAnim>(me->GetMesh()->GetAnimInstance());
	if (anim) {
		anim->OnFire();
	}

	//Sound FX
	UGameplayStatics::PlaySoundAtLocation(GetWorld(), sniperFireSound, me->GetActorLocation(), me->GetActorRotation());

	if (bIsGrenadeLauncher) {
		//when grenade launched
		me->GetWorldTimerManager().SetTimer(fireTimerHandle, this, &UTPSPlayerGunComponent::Fire, fireInterval, true);
		Fire();
	}
	else {
		//when sniper
		FHitResult hitInfo;
		FVector traceStart = me->cameraComp->GetComponentLocation();
		FVector traceEnd = traceStart + me->cameraComp->GetForwardVector() * 100000;

		FCollisionQueryParams params;
		params.AddIgnoredActor(me);


		bool bisHit = GetWorld()->LineTraceSingleByChannel(hitInfo, traceStart, traceEnd, ECollisionChannel::ECC_Visibility, params);

		//if there is something hit
		if (bisHit) {

			//make transform from impact point
			FTransform trans(hitInfo.ImpactPoint);

			UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), bulletImpactFactory, trans);

#pragma region Damage Enemy
			AEnemy* enemy = Cast<AEnemy>(hitInfo.GetActor());

			if (enemy) {
				UEnemyFSM* fsmComponent = Cast<UEnemyFSM>(enemy->GetDefaultSubobjectByName(TEXT("Enemy FSM")));
				fsmComponent->OnDamageProcess(1);
			}
#pragma endregion

			//auto hitComp = hitInfo.GetComponent();
			UPrimitiveComponent* hitComp = hitInfo.GetComponent();

			//Give Physics Interaction
			//if the hitComp is not a nullptr and is simulating physics
			if (hitComp && hitComp->IsSimulatingPhysics()) {

				//Force in the direction from the line trace's start to its end
				//FVector hitForceDir = (hitInfo.TraceEnd - hitInfo.TraceStart).GetSafeNormal();

				FVector hitForceDir = (hitInfo.TraceEnd - hitInfo.TraceStart);
				hitForceDir.Normalize();

				FVector hitForce = hitForceDir * 1000000 * hitComp->GetMass();
				hitComp->AddForce(hitForce);
			}
		}
	}

	//AutoFire
	//GetWorldTimerManager().SetTimer(fireTimerHandle, this, &ATPSPlayer::Fire, fireInterval, true);

	//Fire();


}

void UTPSPlayerGunComponent::OnActionFireReleased()
{
	me->GetWorldTimerManager().ClearTimer(fireTimerHandle);
}


void UTPSPlayerGunComponent::Fire()
{
	FTransform firePosition = me->gunMeshComp->GetSocketTransform(TEXT("FirePosition"));

	firePosition.SetRotation(FQuat(me->GetControlRotation()));

	GetWorld()->SpawnActor<ABulletActor>(bulletFactory, firePosition);
}

void UTPSPlayerGunComponent::ChooseGun(bool bGrenadeLauncher)
{

	//If current gun is a sniper, and I want to change to the grenade launcher
	if (!bIsGrenadeLauncher && bGrenadeLauncher) {
		me->cameraComp->SetFieldOfView(90);
		crossHairUI->AddToViewport();
		sniperCrosshairUI->RemoveFromParent();
	}

	bIsGrenadeLauncher = bGrenadeLauncher;

	me->gunMeshComp->SetVisibility(bGrenadeLauncher);
	me->sniperMeshComp->SetVisibility(!bGrenadeLauncher);

	//Same thing
	//sniperMeshComp->SetVisibility(bChooseGrenadeLauncher ? false : true);
}

void UTPSPlayerGunComponent::OnActionGrenadeLauncher()
{
	ChooseGun(true);
	//Same using macros
	//ChooseGun(GRENADELAUNCHER);
}

void UTPSPlayerGunComponent::OnActionSniper()
{
	ChooseGun(false);

	//Same using macros
	//ChooseGun(SNIPER);
}

void UTPSPlayerGunComponent::OnActionZoomIn()
{
	if (bIsGrenadeLauncher) { return; }

	me->cameraComp->SetFieldOfView(30);

	crossHairUI->RemoveFromParent();
	sniperCrosshairUI->AddToViewport();
}

void UTPSPlayerGunComponent::OnActionZoomOut()
{
	if (bIsGrenadeLauncher) { return; }

	me->cameraComp->SetFieldOfView(90);

	sniperCrosshairUI->RemoveFromParent();
	crossHairUI->AddToViewport();
}

