#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "BaseNPCsInterface.h"
#include "NPCEnemy.generated.h"

class USkeletalMeshComponent;
class AAIController;

UCLASS()
class CEPHEDEKICEVHER_API ANPCEnemy : public ACharacter, public IBaseNPCsInterface{
	GENERATED_BODY()

	public:
		ANPCEnemy();

		virtual void DeathEnemy() override;
		virtual void SetNPCsCollisionContainer() override;

		inline virtual USkeletalMeshComponent *GetNPCMesh() override { return GetMesh(); }
		inline virtual bool GetNPCsIsEnemy() override { return true; }
		inline virtual bool GetNPCsIsFriend() override { return false; }
		inline virtual uint8 GetNPCHealth(void) override { return Health; }
		inline virtual void SetNPCHealth(uint8 newHealth) override { Health -= newHealth; }
		inline virtual bool GetNPCsIsDead() override { return bEnemyIsDead; }
		inline virtual void SetNPCsDeadState(bool newDeadState) override { bEnemyIsDead = newDeadState; }

	private:
		virtual void BeginPlay() override;
		virtual void Tick(float DeltaTime) override;

		UFUNCTION()
			void BodyHit(UPrimitiveComponent *HitComp, AActor *OtherActor, UPrimitiveComponent *OtherComp, FVector NormalImpulse, const FHitResult &Hit);

		void EnemyRespawn();

		uint8 Health = 100;
		bool bEnemyIsDead;
		FTimerHandle RespawnHandle;
		const FVector MoveToTargetLocation = FVector(-2458.026123, 662.231812, -82.166138);
};
