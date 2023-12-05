#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "../Bullets/RocketLauncherBullet.h"
#include "BaseWeaponInterface.h"
#include "WeaponRocketLauncher.generated.h"

class USkeletalMeshComponent;
class UCapsuleComponent;
class UParticleSystem;
class USoundBase;
class USoundAttenuation;
class UAnimationAsset;

class ABaseMagazine;

UCLASS()
class CEPHEDEKICEVHER_API AWeaponRocketLauncher : public AActor, public IBaseWeaponInterface{
	GENERATED_BODY()
	
	public:
		AWeaponRocketLauncher();

		virtual void GunReload() override;
		virtual void SetCollisionContainer() override;
		virtual void SetNoCollision() override;
		virtual UTexture2D *GetWeaponFireModeTexture(uint8 currentMode) override { return nullptr;}
		
		inline virtual void SetCurrentBullet(int16 currentAtilanMermi) override { atilanMermi = currentAtilanMermi; }
		inline virtual void WeaponDestroy() override { this->Destroy(); }
		inline virtual bool GetWeaponIsRocketLauncher() override { return true; }
		inline virtual UClass *GetWeaponUClass() override { return this->StaticClass(); }
		inline virtual USkeletalMeshComponent *GetWeaponMesh() override { return WeaponMesh; }
		inline virtual FString GetWeaponObjectName() override { return this->GetName(); }
		inline virtual int16 GetCurrentBullet() override { return atilanMermi; }
		inline virtual int16 GetMaxBullet() override { return maxMermi; }
		inline virtual UClass *GetWeaponBulletClass() override { return bullet; }
		inline virtual UParticleSystem *GetBulletMuzzleParticle() override { return MuzzleParticle; }
		inline virtual USoundBase *WeaponFireSound() override { return rocketLauncherFireSound; }
		inline virtual FVector GetWeaponBulletSize() override { return FVector(2.5f, 2.5f, 2.5f); }
		inline virtual USoundAttenuation *WeaponFireSoundAttenuation() override { return rocketLauncherSoundAttenuation;}
		inline virtual UTexture2D *GetWeaponTexture() override {return rocketLauncherTexture;}
		inline virtual FString GetWeaponName() override { return FString("RPG"); }
		inline virtual UTexture2D *GetWeaponCrosshair() override { return rocketLauncherCrosshair; }
		inline virtual AWeaponRocketLauncher *GetCurrentLookAtWeapon() override { return this;}
		inline virtual USoundBase *GetWeaponEmptyFireSound() override { return nullptr; }
		inline virtual IBaseWeaponInterface *GetWeaponClass() override { return this; }
		inline virtual USkeletalMeshComponent *GetWeaponHideMesh() override { return WeaponHideMesh; }
		inline virtual UClass *GetWeaponMagazine() override { return nullptr; }
		inline virtual UAnimationAsset *GetWeaponFireAnimation() override { return nullptr;}
		inline virtual UAnimationAsset *GetPistolReloadAnimation(void) override {return nullptr;}
		inline virtual UClass *GetWeaponEjectBulletClass() override { return nullptr; }
		inline virtual void SetCurrentWeaponFireMode(uint8 CurrentWeaponFireMode) override { nullptr; }
		inline virtual uint8 GetCurrentWeaponFireMode() override { return _CurrentWeaponFireMode; }
		inline virtual UParticleSystem *GetRocketLauncherMuzzleRearParticle() override { return MuzzleFlashRear; }
		virtual void CurrentWeaponEjectBullet() override {}

	private:
		uint8 _CurrentWeaponFireMode = 1;
		virtual void BeginPlay() override;
		virtual void Tick(float DeltaTime) override;

		inline virtual bool GetWeaponIsRifle() override { return false; }
		inline virtual bool GetWeaponIsPistol() override { return false; }
		
		UPROPERTY(EditDefaultsOnly,meta=(AllowPrivateAccess = "true"))
			USkeletalMeshComponent *WeaponMesh;

		UPROPERTY(EditDefaultsOnly)
			USkeletalMeshComponent *WeaponHideMesh;
			
		UPROPERTY(EditDefaultsOnly,meta=(AllowPrivateAccess = "true"))
			UCapsuleComponent *CapsuleComponent;

		UPROPERTY(EditDefaultsOnly,Category="Bullet")
			UClass *bullet = ARocketLauncherBullet::StaticClass();
		
		UTexture2D *rocketLauncherCrosshair;
		UTexture2D *rocketLauncherTexture;
		UParticleSystem *MuzzleParticle;
		UParticleSystem *MuzzleFlashRear;
		int16 atilanMermi;
		int16 kalanMermi;
		int16 sarjordekiMermi;
		int16 maxMermi;
		USoundBase *rocketLauncherFireSound;
		USoundAttenuation *rocketLauncherSoundAttenuation;
};
