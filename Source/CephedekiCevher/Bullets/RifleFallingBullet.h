#pragma once

#include "CoreMinimal.h"
#include "BaseBullet.h"
#include "RifleFallingBullet.generated.h"

class USoundBase;

UCLASS()
class CEPHEDEKICEVHER_API ARifleFallingBullet : public ABaseBullet
{
	GENERATED_BODY()
	public:
		ARifleFallingBullet();

		inline virtual UStaticMeshComponent *GetBulletMesh() override { return ammoMesh; }

	private:

		virtual void BeginPlay() override;

		UFUNCTION()
			virtual void BulletHit(UPrimitiveComponent *HitComponent, AActor *OtherActor, UPrimitiveComponent *OtherComp, FVector NormalImpulse, const FHitResult &Hit) override;

		USoundBase *FallingSound;
};
