#pragma once

#include "CoreMinimal.h"
#include "BaseBullet.h"
#include "PistolFallingBullet.generated.h"

class USoundBase;

UCLASS()
class CEPHEDEKICEVHER_API APistolFallingBullet : public ABaseBullet
{
	GENERATED_BODY()
	public:
		APistolFallingBullet();

		inline virtual UStaticMeshComponent *GetBulletMesh() override { return ammoMesh; }

	private:

		virtual void BeginPlay() override;

		UFUNCTION()
			virtual void BulletHit(UPrimitiveComponent *HitComponent, AActor *OtherActor, UPrimitiveComponent *OtherComp, FVector NormalImpulse, const FHitResult &Hit) override;

		USoundBase *FallingSound;
};
