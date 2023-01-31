// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyAnim.h"

#include "Enemy.h"


void UEnemyAnim::NativeBeginPlay()
{
	Super::NativeBeginPlay();
	
	me = Cast<AEnemy>(TryGetPawnOwner());
}


// void UEnemyAnim::OnHitEvent()
// {
// 	//call enemyFSM's OnHitEvent
// 	me->enemyFSM->OnHitEvent();
// }

void UEnemyAnim::AnimNotify_OnHit()
{
	me->enemyFSM->OnHitEvent();
}
