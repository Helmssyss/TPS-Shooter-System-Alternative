#pragma once

#include "CoreMinimal.h"
#include "BaseBullet.h"
#include "RifleBullet.generated.h"

class UProjectileMovementComponent;
class UStaticMeshComponent;

UCLASS()
class CEPHEDEKICEVHER_API ARifleBullet : public ABaseBullet{
	GENERATED_BODY()

	public:
		ARifleBullet();

		inline virtual UStaticMeshComponent *GetBulletMesh() override { return ammoMesh; }
		inline virtual UProjectileMovementComponent *GetBulletProjectileComponent() override { return projectileMovement; }
		inline virtual void DestroyBullet() override { this->Destroy(); }

	private:
		virtual void BeginPlay() override;
		virtual void EndPlay(EEndPlayReason::Type EndPlayReason) override;
		virtual void BulletImpactParticle(UPrimitiveComponent *OtherComp, const FHitResult &Hit) override;
		virtual void BulletHitAndHide() override;

		UFUNCTION()
			virtual void BulletHit(UPrimitiveComponent *HitComponent, AActor *OtherActor, UPrimitiveComponent *OtherComp, FVector NormalImpulse, const FHitResult &Hit) override;
};