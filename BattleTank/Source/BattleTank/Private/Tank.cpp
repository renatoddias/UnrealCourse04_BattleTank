// Copyright EmbraceIT Ltd.

#include "Tank.h"
#include "TankBarrel.h"
#include "TankAimingComponent.h"
#include "Projectile.h"
#include "Engine/World.h"


// Sets default values
ATank::ATank()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	// No need to protect pointers as added at construction
	TankBody = CreateDefaultSubobject<UStaticMeshComponent>(FName("Tank Body"));
	RootComponent = TankBody;
}

void ATank::Fire()
{
	if (!ensure(ProjectileBlueprint)) return;
	auto Barrel = this->FindComponentByClass<UTankBarrel>();
	if (!ensure(Barrel)) return;

	bool isReloaded = (FPlatformTime::Seconds() - LastFireTime) > ReloadTimeInSeconds;
	if (isReloaded)
	{
		auto Projectile = GetWorld()->SpawnActor<AProjectile>(
			ProjectileBlueprint,
			Barrel->GetSocketLocation(FName("Projectile")),
			Barrel->GetSocketRotation(FName("Projectile"))
			);

		Projectile->LaunchProjectile(this->FindComponentByClass<UTankAimingComponent>()->LaunchSpeed);
		LastFireTime = FPlatformTime::Seconds();
	}
}