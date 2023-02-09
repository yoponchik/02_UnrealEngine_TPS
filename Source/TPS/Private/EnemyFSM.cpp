// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyFSM.h"
#include <Kismet/GameplayStatics.h>
#include "TPSPlayer.h"
#include "Enemy.h"
#include <GameFramework/CharacterMovementComponent.h>
#include "../TPS.h"
#include <Components/CapsuleComponent.h>
#include "EnemyAnim.h"
#include "AIController.h"
#include "NavigationSystem.h"
#include "PathManager.h"
#include "TPS/TPSGameModeBase.h"

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
	aI = Cast<AAIController>(me->GetController());
	//add
	me->GetCharacterMovement()->bOrientRotationToMovement = true;

	me->enemyHP = me->enemyMaxHP;

	//Navigation
	//Create random destination when spawned
	UpdateRandomLocation(randomLocationRadius, randomLocation);

	moveSubState = EEnemyMoveSubState::PATROL;

	//waypoint
	pathManager = Cast<APathManager>(UGameplayStatics::GetActorOfClass(GetWorld(), APathManager::StaticClass()));	
}


// Called every frame
void UEnemyFSM::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...

	//Continuously update EnemyAnim State with EnemyFSM State

	me->enemyAnim->enemyState = enemyState;
	
	switch (enemyState)
	{
	case EEnemyState::IDLE:
		OnTickIdle();
		//SetAnimState(EEnemyState::IDLE);
		break;
	case EEnemyState::MOVE:
		OnTickMove();
		//SetAnimState(EEnemyState::MOVE);
		break;
	case EEnemyState::ATTACK:
		OnTickAttack();
		//SetAnimState(EEnemyState::ATTACK);
		break;
	case EEnemyState::DAMAGE:
		OnTickDamage();
		//SetAnimState(EEnemyState::DAMAGE);
		break;
	case EEnemyState::DIE:
		OnTickDie();
		//SetAnimState(EEnemyState::DIE);
		break;
	}
}

void UEnemyFSM::SetState(EEnemyState next)
{
	enemyState = next;
	me->enemyAnim->enemyState = next;

	currentTime = 0;
}

void UEnemyFSM::OnTickOldMove()
{
#pragma region Navigation - Using Navigation Invoker
	//check if there is a target on the generated navmesh
	UNavigationSystemV1* nS = UNavigationSystemV1::GetNavigationSystem(GetWorld());

	FPathFindingQuery query;
	FAIMoveRequest request;

	//make a pathfinding request
	request.SetGoalLocation(target->GetActorLocation());		//Set Navigation goal location
	request.SetAcceptanceRadius(5);								 //The error tolerance distance

	//make a pathfinding query from request
	aI->BuildPathfindingQuery(request, query);				//getting query from request

	//get the pathfinding request result from query
	FPathFindingResult result = nS->FindPathSync(query);
	
	//if query result is a success
	if(result.Result == ENavigationQueryResult::Success){
		//Move to that target
		aI->MoveToLocation(target->GetActorLocation());				
	}
	//if target not on navigation, go to random location
	else{
		
		auto navigationStatus = aI->MoveToLocation(randomLocation);
		if(navigationStatus == EPathFollowingRequestResult::AlreadyAtGoal || navigationStatus == EPathFollowingRequestResult::Failed){
			//if arrived at location, choose new random location
			UpdateRandomLocation(randomLocationRadius, randomLocation);
		}
		
	}
#pragma endregion Nav Invoker
}

void UEnemyFSM::OnTickPatrol()
{
	FVector patrolDestination = pathManager->wayPointsArray[wayPtIndex]->GetActorLocation();
	auto result = aI->MoveToLocation(patrolDestination);
	if(result == EPathFollowingRequestResult::AlreadyAtGoal || result == EPathFollowingRequestResult::Failed){
		//Numerical Order - Ascending Method 1
		/*wayPtIndex++;
		if(wayPtIndex >= pathManager->wayPointsArray.Num()){
			wayPtIndex = 0;
		}*/

		//Numerical Order - Ascending Method 2
		//wayPtIndex= (wayPtIndex + 1) % pathManager->wayPointsArray.Num();

		//Numerical Order - Descending Method 
		//wayPtIndex = (wayPtIndex + pathManager->wayPointsArray.Num() - 1) % pathManager->wayPointsArray.Num();

		
		//Random Order 
		//wayPtIndex = FMath::RandRange(0, pathManager->wayPointsArray.Num() -1);
		//same thing
		wayPtIndex = FMath::RandRange(0, pathManager->wayPointsArray.Num() - 1);
	}

	//Transition to Chase
	//check the distance between the target and this
	float distance = me->GetDistanceTo(target);
	//if the target is within the detectionDistance,
	if(distance < detectionDistance){	
		//transition to Chase State
		moveSubState = EEnemyMoveSubState::CHASE;
	}
}

void UEnemyFSM::OnTickChase()
{
	aI->MoveToLocation(target->GetActorLocation());

	//get direction to target
	FVector dir = target->GetActorLocation() - me->GetActorLocation();

	//get distance between target and AI
	float dist = dir.Size();				//it's okay to do this after normalizing, because we did GetSafeNormal, which copied it
	//Same thing
	//float dist = target->GetDistanceTo(me);
	//Same thing
	//float dist = FVector::Dist(target->GetActorLocation(), me->GetActorLocation());

	//Transition to Attack
	//if within the attack range
	if (dist <= attackDistance) {
		//enemyState = EEnemyState::ATTACK;
		SetState(EEnemyState::ATTACK);
	}

	//Transition to Patrol
	else if(dist > abandonDistance){
		moveSubState = EEnemyMoveSubState::PATROL;
	}
}

void UEnemyFSM::OnTickMove()
{
	switch (moveSubState){
	case EEnemyMoveSubState::PATROL:
		OnTickPatrol();
		break;
	case EEnemyMoveSubState::CHASE:
		OnTickChase();
		break;
	case EEnemyMoveSubState::OLD_MOVE:
		OnTickOldMove();
		break;
	}
	

}

//when the enemy attacks the player
void UEnemyFSM::OnHitEvent()
{
	me->enemyAnim->bAttackPlay = false;							//WAIT FOR ATTACK
	//3.0 make bAttackPlay = true
	//3. if target is inside the attackDistance
	float distance = target->GetDistanceTo(me);
	if (distance <= attackDistance) {
		//4. attack
		target->OnTakeDamage(1);
	}
}

bool UEnemyFSM::UpdateRandomLocation(float radius, FVector& outLocation)
{
	UNavigationSystemV1* randomNS = UNavigationSystemV1::GetNavigationSystem(GetWorld());

	FNavLocation navLocation;
	
	bool result = randomNS->GetRandomReachablePointInRadius(me->GetActorLocation(), radius, navLocation);

	if(result){
		outLocation = navLocation;
	}
	return result;
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

	//enemyState = EEnemyState::MOVE;
	SetState(EEnemyState::MOVE);

	//2. if player is found
	//3. transition to Move
}



void UEnemyFSM::OnTickAttack()
{
	//1. time runs
	currentTime += GetWorld()->GetDeltaSeconds(); 


	//5. if the attack action is finished
	if (currentTime > attackDelayTime) {
	//6. Check if should attack or not
		//6. if far from attackDistance, change to move
		float distance = target->GetDistanceTo(me);
		if (distance > attackDistance) {
			//4. move
			SetState(EEnemyState::MOVE);
		}
		else {	//if inside the attackdistance
			currentTime = 0;
			bAttackPlay = false;					
			me->enemyAnim->bAttackPlay = true;						//ATTACK
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
		//enemyState = EEnemyState::IDLE;
		SetState(EEnemyState::MOVE);
		currentTime = 0;
	}
}

//player->enemyaa
//get hit by player
void UEnemyFSM::OnDamageProcess(int damageAmount)
{
	//hp decreases
	me->enemyHP -= damageAmount;
	//if less than 0
	if (me->enemyHP <= 0) {
		me->enemyHP = 0;						//to change negative HP into 0
		//enemyState = EEnemyState::DIE;
		SetState(EEnemyState::DIE);

		me->enemyAnim->bEnemyDieEnd = false;
		me->OnMyDamage(TEXT("Die"));
		
		me->GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);

		//Call GameMode's AddEXP function. add 1
		Cast<ATPSGameModeBase>(GetWorld()->GetAuthGameMode())->AddEXP(1);
	}
	else {
		//enemyState = EEnemyState::DAMAGE;
		SetState(EEnemyState::DAMAGE);

		int index = FMath::RandRange(0,1);
		FString sectionName = FString::Printf(TEXT("Damage%d"), index);

		me->OnMyDamage(FName(*sectionName));

		#pragma region Random Damage Method 1
		/*if(FMath::RandRange(0, 100) > 50){
			me->OnMyDamage((TEXT("Damage0")));
		}
		else{
			me->OnMyDamage((TEXT("Damaage1")));
		}*/
		#pragma endregion Random Damage Method 1
	}
}

void UEnemyFSM::OnTickDie()
{
	//if bEnemyDieEnd is false, don't do anything
	if(!(me->enemyAnim->bEnemyDieEnd)){
		return;
	}

	//if bEnemyDieEnd flag is true, die
	currentTime += GetWorld()->GetDeltaSeconds();

	FVector p0 = me->GetActorLocation();
	FVector velocity = FVector::DownVector * 200 * GetWorld()->GetDeltaSeconds();

	me->SetActorLocation(p0 + velocity);

	if (currentTime > 1) {
		me->Destroy();
		currentTime = 0;
	}
}
