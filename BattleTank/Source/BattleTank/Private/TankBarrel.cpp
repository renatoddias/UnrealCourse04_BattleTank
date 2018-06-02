// Fill out your copyright notice in the Description page of Project Settings.

#include "TankBarrel.h"

void UTankBarrel::Elevate(float RelativeSpeed)
{

	// move the barrel the right amoun
	// given a max elevation speed and frame time
	auto Time = GetWorld()->GetTimeSeconds();
	UE_LOG(LogTemp, Warning, TEXT("%f: Barrel->Elevate() called at speed %f"), Time, RelativeSpeed);
}


