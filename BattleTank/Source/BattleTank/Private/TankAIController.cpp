// Copyright EmbraceIT Ltd.

#include "TankAIController.h"
#include "Tank.h"
#include "TankAimingComponent.h"
#include "Engine/World.h"

void ATankAIController::BeginPlay()
{
	Super::BeginPlay();

}

void ATankAIController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	auto PlayerTank = Cast<ATank>(GetWorld()->GetFirstPlayerController()->GetPawn());

	if (PlayerTank)
	{
		// TODO Move towards the player
		MoveToActor(PlayerTank, AcceptanceRadius);

		// Aim towards the player
		auto ControlledTank = Cast<ATank>(GetPawn());
		auto AimingComponent = ControlledTank->FindComponentByClass<UTankAimingComponent>();
		if (!AimingComponent) return;
		AimingComponent->AimAt(PlayerTank->GetActorLocation());

		//ControlledTank->Fire();
	}
}