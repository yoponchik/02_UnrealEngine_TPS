// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyFSM.h"
#include <Kismet/GameplayStatics.h>
#include "TPSPlayer.h"
#include "Enemy.h"
#include <GameFramework/CharacterMovementComponent.h>
#include "../TPS.h"
#include <Components/CapsuleComponent.h>

// Sets default values for this component's properties
UEnemyFSM::UEnemyFSM()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UEnemyFSM::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
	enemyState = EEnemyState::IDLE;

	me = Cast<AEnemy>(GetOwner());
	//add
	me->GetCharacterMovement()->bOrientRotationToMovement = true;

	enemyHP = enemyMaxHP;
}


// Called every frame
void UEnemyFSM::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...

	switch (enemyState)
	{
	case EEnemyState::IDLE:
		OnTickIdle();
		break;
	case EEnemyState::MOVE:
		OnTickMove();
		break;
	case EEnemyState::ATTACK:
		OnTickAttack();
		break;
	case EEnemyState::DAMAGE:
		OnTickDamage();
		break;
	case EEnemyState::DIE:
		OnTickDie();
		break;
	}
}

void UEnemyFSM::OnTickMove()
{

	FVector dir = target->GetActorLocation() - me->GetActorLocation();
	me->AddMovementInput(dir.GetSafeNormal());

	float dist = dir.Size();				//it's okay to do this after normalizing, because we did GetSafeNormal, which copied it
	//Same thing
	//float dist = target->GetDistanceTo(me);
	//Same thing
	//float dist = FVector::Dist(target->GetActorLocation(), me->GetActorLocation());

	if (dist <= attackDistance) {
		enemyState = EEnemyState::ATTACK;
	}
}


/// <summary>
/// Idle; Transition to Move after finding player
/// </summary>
void UEnemyFSM::OnTickIdle()
{
	//1. Find Player
	//auto target = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);
	//APawn* target = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);
	target = Cast<ATPSPlayer>(UGameplayStatics::GetPlayerPawn(GetWorld(), 0));
	
	if (!target) { return; }				//Note that this is after the cast; or else if it's before the cast, it won't run the rest of the function

	enemyState = EEnemyState::MOVE;
	
	//2. if player is found
	//3. transition to Move
}



void UEnemyFSM::OnTickAttack()
{
	//1. time runs
	currentTime += GetWorld()->GetDeltaSeconds(); 
	//2. if currentTime exceeds attacktime && bAttackPlay animation is not done yet
	if (!bAttackPlay && currentTime > 0.1f) {
		//3.0 make bAttackPlay = true
		bAttackPlay = true;
		//3. if target is inside the attackDistance
		float distance = target->GetDistanceTo(me);
		if (distance <= attackDistance) {
		//4. attack
			PRINT_LOG(TEXT("Enemy Attack"));
		}
	}


	//5. if the attack action is finished
	if (currentTime > 2) {
	//6. Check if should attack or not
		//6. if far from attackDistance, change to move
		float distance = target->GetDistanceTo(me);
		if (distance > attackDistance) {
			//4. move
			enemyState = EEnemyState::MOVE;
		}
		else {	//if inside the attackdistance
			currentTime = 0;
			bAttackPlay = false;
		}
	}

	#pragma region Alternative - Can be Buggy
	//Possible bugginess because it has to go through all the other states; animation might be weird
		//if(currentTime > 2){
	//	bAttackPlay = false;
	//	currentTime = 0;
	//	enemyState = EEnemyState::IDLE;
	//}
#pragma endregion

}



void UEnemyFSM::OnTickDamage()
{
	currentTime += GetWorld()->GetDeltaSeconds();

	if (currentTime > 1) {
		enemyState = EEnemyState::IDLE;
		currentTime = 0;
	}
}

//player->enemyaa
//get hit by player
void UEnemyFSM::OnDamageProcess(int damageAmount)
{
	//hp decreases
	enemyHP -= damageAmount;
	//if less than 0
	if (enemyHP <= 0) {
		enemyState = EEnemyState::DIE;
		me->GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	}
	else {
		enemyState = EEnemyState::DAMAGE;
	}
}

void UEnemyFSM::OnTickDie()
{
	currentTime += GetWorld()->GetDeltaSeconds();

	FVector p0 = me->GetActorLocation();
	FVector velocity = FVector::DownVector * 200 * GetWorld()->GetDeltaSeconds();

	me->SetActorLocation(p0 + velocity);

	if (currentTime > 1) {
		me->Destroy();
		currentTime = 0;
	}
}
