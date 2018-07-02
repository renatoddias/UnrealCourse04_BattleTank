// Copyright EmbraceIT Ltd.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/NavMovementComponent.h"
#include "TankMovementComponent.generated.h"

class UTankTrack;

 // Holds fly-by-wire movement method
UCLASS( ClassGroup = (Custom), meta = (BlueprintSpawnableComponent) )
class BATTLETANK_API UTankMovementComponent : public UNavMovementComponent
{
	GENERATED_BODY()
	
public:
	UFUNCTION(BlueprintCallable, Category = "Setup")
	void Initialise(UTankTrack * LeftTrackToSet, UTankTrack * RightTrackToSet);

	UFUNCTION(BlueprintCallable, Category = "Input")
	void IntendMoveForward(float Throw);

	UFUNCTION(BlueprintCallable, Category = "Input")
	void IntendRotateClockwise(float Throw);

private:
	virtual void RequestDirectMove(const FVector& MoveVelocity, bool bForceMaxSpeed) override;

	UTankTrack * LeftTrack = nullptr;
	UTankTrack * RightTrack = nullptr;
};