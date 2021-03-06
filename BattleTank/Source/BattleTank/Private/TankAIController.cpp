// Copyright EmbraceIT Ltd.

#include "TankAIController.h"
#include "TankAimingComponent.h"
#include "Tank.h"
#include "Engine/World.h"

void ATankAIController::BeginPlay()
{
	Super::BeginPlay();

}

void ATankAIController::SetPawn(APawn * InPawn)
{
	Super::SetPawn(InPawn);

	if (InPawn)
	{
		auto PossessedTank = Cast<ATank>(InPawn);
		if (!ensure(PossessedTank)) return; 

		PossessedTank->OnDeath.AddUniqueDynamic(this, &ATankAIController::OnPossessedTankDeath);
	}
}


void ATankAIController::OnPossessedTankDeath()
{
	if (!GetPawn()) return;

	GetPawn()->DetachFromControllerPendingDestroy();
}


void ATankAIController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	auto ControlledTank = GetPawn();
	if (!ensure(ControlledTank)) return;
	auto AimingComponent = ControlledTank->FindComponentByClass<UTankAimingComponent>();
	if (!ensure(AimingComponent)) return;
	auto PlayerTank = GetWorld()->GetFirstPlayerController()->GetPawn();
	if (!ensure(PlayerTank)) return;
	
	// TODO Move towards the player
	MoveToActor(PlayerTank, AcceptanceRadius);

	// Aim towards the player
	AimingComponent->AimAt(PlayerTank->GetActorLocation());

	if (AimingComponent->GetFiringState() == EFiringState::Locked)
	{
		AimingComponent->Fire();
	}
}