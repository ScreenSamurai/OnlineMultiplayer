// Fill out your copyright notice in the Description page of Project Settings.


#include "MovingPlatform.h"

AMovingPlatform::AMovingPlatform()
{
	PrimaryActorTick.bCanEverTick = true;

	SetMobility(EComponentMobility::Movable);

}

void AMovingPlatform::BeginPlay()
{
	Super::BeginPlay();

	if (HasAuthority())
	{
	SetReplicates(true);
	SetReplicateMovement(true);
	}
	GlobalStartLocation = GetActorLocation();
	GlobalTargetLocation = GetTransform().TransformPosition(TargetLocation);
}

void AMovingPlatform::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (ActiveTigger>0)
	{
		if (HasAuthority())
		{
			FVector Location = GetActorLocation();
			float JurneyLenght = (GlobalTargetLocation - GlobalStartLocation).Size();
			float JurneyTravelled = (Location - GlobalStartLocation).Size();
			if (JurneyTravelled >= JurneyLenght) 
			{
				FVector Swap = GlobalStartLocation;
				GlobalStartLocation = GlobalTargetLocation;
				GlobalTargetLocation = Swap;
			}
		FVector Direction = (GlobalTargetLocation - Location).GetSafeNormal();
		Location += SpeedMovePlatform  * DeltaTime * Direction;
		SetActorLocation(Location);
	}

	}
}

void AMovingPlatform::RemoveActionTrigger()
{
	ActiveTigger++;
}

void AMovingPlatform::AddActiveTrigger()
{
	if (ActiveTigger > 0)
	{
		ActiveTigger--;
	}
}

