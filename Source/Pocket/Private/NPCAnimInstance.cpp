// Copyright Daniel Sellers 2024.


#include "NPCAnimInstance.h"
#include "GameFramework/Character.h"
#include "Pocket/Pocket.h"

void UNPCAnimInstance::NativeUpdateAnimation(float DeltaTime)
{
	Super::NativeUpdateAnimation(DeltaTime);

	APawn* Pawn = TryGetPawnOwner();

	if (!IsValid(Pawn))
	{
		//UE_LOG(LogPocket, Error, TEXT("NPCAnimInstance: Failed to get pawn"));
		return;
	}
	
	Velocity = Pawn->GetVelocity();
	Speed = Velocity.Size();
}
