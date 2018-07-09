// Copyright EmbraceIT Ltd.

#include "TankAimingComponent.h"
#include "Engine/World.h"
#include "TankBarrel.h"
#include "TankTurret.h"
#include "Projectile.h"
#include "Kismet/GameplayStatics.h"


// Sets default values for this component's properties
UTankAimingComponent::UTankAimingComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	
}

void UTankAimingComponent::BeginPlay()
{
	LastFireTime = FPlatformTime::Seconds();
}

void UTankAimingComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction * ThisTickFunction)
{
	if ((FPlatformTime::Seconds() - LastFireTime) > ReloadTimeInSeconds)
	{
		FiringState = EFiringState::Reloading;
	}
}

void UTankAimingComponent::Initialize(UTankBarrel * BarrelToSet, UTankTurret * TurretToSet)
{
	Barrel = BarrelToSet;
	Turret = TurretToSet;
}

void UTankAimingComponent::AimAt(FVector HitLocation)
{
	if (!ensure(Barrel)) return;
	if (!ensure(Turret)) return;

	FVector OutLaunchVelocity;
	FVector StartLocation = Barrel->GetSocketLocation(FName("Projectile"));
	bool bHaveAimSolution = UGameplayStatics::SuggestProjectileVelocity
	(
		this,
		OutLaunchVelocity,
		StartLocation,
		HitLocation,
		LaunchSpeed,
		false,
		0,
		0,
		ESuggestProjVelocityTraceOption::DoNotTrace
	);
	if (bHaveAimSolution)		
	{
		auto AimDirection = OutLaunchVelocity.GetSafeNormal();
		MoveBarrelAndTurret(AimDirection);
		//auto Time = GetWorld()->GetTimeSeconds();
		//UE_LOG(LogTemp, Warning, TEXT("%f: Aim solution found"), Time);
	} else
	{
		//auto Time = GetWorld()->GetTimeSeconds();
		//UE_LOG(LogTemp, Warning, TEXT("%f: Aim solution not found"), Time);
	}
}

void UTankAimingComponent::MoveBarrelAndTurret(FVector AimDirection)
{
	// Work-out difference between current rotation and aim direction

	auto BarrelRotator = Barrel->GetForwardVector().Rotation();
	auto TurretRotator = Turret->GetForwardVector().Rotation();
	auto AimAsRotator = AimDirection.Rotation();
	auto BarrelDeltaRotator = AimAsRotator - BarrelRotator;
	auto TurretDeltaRotator = AimAsRotator - TurretRotator;

	Barrel->Elevate(BarrelDeltaRotator.Pitch);
	Turret->Rotate(TurretDeltaRotator.Yaw);
}

void UTankAimingComponent::Fire()
{
	if (!ensure(ProjectileBlueprint)) return;
	if (!ensure(Barrel)) return;

	if (FiringState != EFiringState::Reloading)
	{
		auto Projectile = GetWorld()->SpawnActor<AProjectile>(
			ProjectileBlueprint,
			Barrel->GetSocketLocation(FName("Projectile")),
			Barrel->GetSocketRotation(FName("Projectile"))
			);

		Projectile->LaunchProjectile(LaunchSpeed);
		LastFireTime = FPlatformTime::Seconds();
	}
}