#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BaseBullet.generated.h"

class UStaticMeshComponent;
class UProjectileMovementComponent;
class UParticleSystemComponent;
class USphereComponent;
class UParticleSystem;
class UAudioComponent;
class URotatingMovementComponent;
class UBulletHitWidget;
class USoundBase;
class USoundAttenuation;

UCLASS()
class CEPHEDEKICEVHER_API ABaseBullet : public AActor{
	GENERATED_BODY()
	
	public:
		ABaseBullet();
		virtual UStaticMeshComponent *GetBulletMesh() { return nullptr; }
		virtual UProjectileMovementComponent *GetBulletProjectileComponent() { return nullptr; }
		virtual void DestroyBullet(){}
		
		UPROPERTY()
			UBulletHitWidget *BulletWidget;

	protected:
		virtual void BulletHit(UPrimitiveComponent *HitComponent, AActor *OtherActor, UPrimitiveComponent *OtherComp, FVector NormalImpulse, const FHitResult &Hit){}
		virtual void BulletImpactParticle(UPrimitiveComponent *OtherComp, const FHitResult &Hit){}
		virtual void BulletHitAndHide(){}

		UPROPERTY(EditDefaultsOnly,BlueprintReadOnly)
			UStaticMeshComponent *ammoMesh;

		UPROPERTY(EditDefaultsOnly)
			UProjectileMovementComponent *projectileMovement;
		
		UPROPERTY(EditDefaultsOnly)
			USphereComponent* ammoCapsule;
		
		UPROPERTY(EditDefaultsOnly)
			UParticleSystemComponent *BulletTrail;
		
		UPROPERTY(EditDefaultsOnly)
			UAudioComponent* BulletAudioCurve;
		
		UPROPERTY(EditDefaultsOnly)
			URotatingMovementComponent *BulletRotatingMovementComponent;

		UPROPERTY(EditDefaultsOnly,Category="Bullet Widgets",meta=(AllowPrivateAccess = "true"))
			TSubclassOf<UBulletHitWidget> TBulletWidget;
		
		FTimerHandle BulletTimerHandle;
		USoundBase *BodyHitSound;
		USoundBase *BulletImpactEmitterSound;
		USoundAttenuation *BulletSoundAttenuation;
		UParticleSystem *BulletImpactEmitterTemplate;
		UParticleSystem *BulletBodyImpactEmitterTemplate;
		UParticleSystem *BulletDefaultImpactEmitterTemplate;
		UMaterialInterface *BulletHoleDecalMaterial;
		UMaterialInterface *BulletBloodDecalMaterial;
};
