#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"

#include "../Bullets/PistolBullet.h"
#include "../Bullets/PistolFallingBullet.h"
#include "../CollectibleItems/PistolMagazineItem.h"

#include "BaseWeaponInterface.h"
#include "WeaponPistol.generated.h"

class USkeletalMeshComponent;
class UCapsuleComponent;
class UParticleSystem;
class USoundBase;
class USoundAttenuation;
class UAnimationAsset;

class ABaseMagazine;

UCLASS()
class CEPHEDEKICEVHER_API AWeaponPistol : public AActor, public IBaseWeaponInterface{
	GENERATED_BODY()
	
	public:	
		AWeaponPistol();

		virtual void GunReload() override;
		virtual void SetCollisionContainer() override;
		virtual void SetNoCollision() override;
		
		inline virtual void SetCurrentBullet(int16 currentAtilanMermi) override { atilanMermi = currentAtilanMermi; }
		inline virtual void WeaponDestroy() override { this->Destroy(); }
		inline virtual bool GetWeaponIsPistol() override { return true; }
		inline virtual UClass *GetWeaponUClass() override { return this->StaticClass(); }
		inline virtual USkeletalMeshComponent *GetWeaponMesh() override { return WeaponMesh; }
		inline virtual FString GetWeaponObjectName() override { return this->GetName(); }
		inline virtual int16 GetCurrentBullet() override { return atilanMermi; }
		inline virtual int16 GetMaxBullet() override { return maxMermi; }
		inline virtual UClass *GetWeaponBulletClass() override { return bullet; }
		inline virtual UParticleSystem *GetBulletMuzzleParticle() override { return MuzzleParticle; }
		inline virtual USoundBase *WeaponFireSound() override { return pistolFireSound; }
		inline virtual FVector GetWeaponBulletSize() override { return FVector(1, 1, 1); }
		inline virtual USoundAttenuation *WeaponFireSoundAttenuation() override { return pistolSoundAttenuation;}
		inline virtual UTexture2D *GetWeaponTexture() override {return pistolTexture;}
		inline virtual FString GetWeaponName() override { return FString("Pistol");}
		inline virtual UTexture2D *GetWeaponCrosshair() override { return pistolCrosshair;}
		inline virtual AWeaponPistol *GetCurrentLookAtWeapon() override { return this;}
		inline virtual USoundBase *GetWeaponEmptyFireSound() override { return pistolEmptySound; }
		inline virtual IBaseWeaponInterface *GetWeaponClass() override { return this; }
		inline virtual USkeletalMeshComponent *GetWeaponHideMesh() override { return WeaponHideMesh; }
		inline virtual UClass *GetWeaponMagazine() override { return MagazineClass; }
		inline virtual UAnimationAsset *GetWeaponFireAnimation() override { return pistolFireAnimation;}
		inline virtual UAnimationAsset *GetPistolReloadAnimation(void) override {return pistolReloadAnimation;}
		inline virtual UClass *GetWeaponEjectBulletClass() override { return ejectBullet; }
		inline virtual void SetCurrentWeaponFireMode(uint8 CurrentWeaponFireMode) override { nullptr; }
		inline virtual uint8 GetCurrentWeaponFireMode() override { return _CurrentWeaponFireMode; }
		inline virtual UTexture2D *GetWeaponFireModeTexture(uint8 currentMode) override { return pistolFireModeTexture;}
		virtual void CurrentWeaponEjectBullet() override;

	private:
		uint8 _CurrentWeaponFireMode = 1;
		virtual void BeginPlay() override;
		virtual void Tick(float DeltaTime) override;
		
		inline virtual bool GetWeaponIsRifle() override { return false; }
		inline virtual bool GetWeaponIsRocketLauncher() override { return false; }

		UPROPERTY(EditDefaultsOnly)
			USkeletalMeshComponent *WeaponMesh;

		UPROPERTY(EditDefaultsOnly)
			USkeletalMeshComponent *WeaponHideMesh;
			
		UPROPERTY(EditDefaultsOnly)
			UCapsuleComponent *CapsuleComponent;

		UPROPERTY(EditDefaultsOnly,Category="Bullet")
			UClass *ejectBullet = APistolFallingBullet::StaticClass();

		UPROPERTY(EditDefaultsOnly,Category="Bullet")
			UClass *bullet = APistolBullet::StaticClass();

		UPROPERTY(EditDefaultsOnly,Category="Magazine")
			UClass *MagazineClass = APistolMagazineItem::StaticClass();

		UAnimationAsset *pistolFireAnimation;
		UAnimationAsset *pistolReloadAnimation;
		UTexture2D *pistolCrosshair;
		UTexture2D *pistolTexture;
		UTexture2D *pistolFireModeTexture;
		UParticleSystem *MuzzleParticle;
		int16 atilanMermi;
		int16 kalanMermi;
		int16 sarjordekiMermi;
		int16 maxMermi;
		USoundBase *pistolFireSound;
		USoundBase *pistolEmptySound;
		USoundAttenuation *pistolSoundAttenuation;
};