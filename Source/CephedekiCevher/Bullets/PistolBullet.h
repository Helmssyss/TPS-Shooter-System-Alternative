#pragma once

#include "CoreMinimal.h"
#include "BaseBullet.h"
#include "PistolBullet.generated.h"

class UProjectileMovementComponent;
class UStaticMeshComponent;

UCLASS()
class CEPHEDEKICEVHER_API APistolBullet : public ABaseBullet{
	GENERATED_BODY()
	
	public:
		APistolBullet();

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
