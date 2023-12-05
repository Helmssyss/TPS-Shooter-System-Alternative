#pragma once

#include "CoreMinimal.h"
#include "BaseBullet.h"
#include "RocketLauncherBullet.generated.h"

class UParticleSystem;
class USoundBase;
class USoundAttenuation;

UCLASS()
class CEPHEDEKICEVHER_API ARocketLauncherBullet : public ABaseBullet
{
	GENERATED_BODY()
	public:
		ARocketLauncherBullet();
		inline virtual UStaticMeshComponent *GetBulletMesh() override { return ammoMesh; }
		inline virtual UProjectileMovementComponent *GetBulletProjectileComponent() override { return projectileMovement; }
		inline virtual void DestroyBullet() override { this->Destroy(); }

	private:
		virtual void BeginPlay() override;
		virtual void EndPlay(EEndPlayReason::Type EndPlayReason) override;
		virtual void BulletHitAndHide() override;

		UFUNCTION()
			virtual void BulletHit(UPrimitiveComponent *HitComponent, AActor *OtherActor, UPrimitiveComponent *OtherComp, FVector NormalImpulse, const FHitResult &Hit) override;

		UFUNCTION()
			void OnAudioFinished();

		void PlayAudio();

		FTimerHandle TimerHandle_AudioRestart;
		UParticleSystem *BulletHitExplosionParticle;
		USoundBase *ExplosionSound;
		USoundAttenuation *ExplosionAttenuationSound;
};
