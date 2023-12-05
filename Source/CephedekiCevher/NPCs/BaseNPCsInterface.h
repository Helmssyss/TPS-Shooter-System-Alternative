#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "BaseNPCsInterface.generated.h"

class USkeletalMeshComponent;

UINTERFACE(MinimalAPI)
class UBaseNPCsInterface : public UInterface{GENERATED_BODY()};

class CEPHEDEKICEVHER_API IBaseNPCsInterface{
	GENERATED_BODY()
	public:
		virtual bool GetNPCsIsEnemy(void) = 0;
		virtual bool GetNPCsIsFriend(void) = 0;
		virtual uint8 GetNPCHealth(void) = 0;
		virtual void SetNPCHealth(uint8 newHealth) = 0;
		virtual USkeletalMeshComponent *GetNPCMesh(void) = 0;
		virtual void DeathEnemy() = 0;
		virtual bool GetNPCsIsDead() = 0;
		virtual void SetNPCsDeadState(bool newDeadState) = 0;
		virtual void SetNPCsCollisionContainer() = 0;
};
