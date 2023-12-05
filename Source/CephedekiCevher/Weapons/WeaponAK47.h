#pragma once

#include "CoreMinimal.h"

#include "GameFramework/Actor.h"
#include "BaseWeaponInterface.h"

#include "../Bullets/RifleBullet.h"
#include "../Bullets/RifleFallingBullet.h"
#include "../CollectibleItems/AK47MagazineItem.h"

#include "WeaponAK47.generated.h"

class USkeletalMeshComponent;
class UCapsuleComponent;
class UParticleSystem;
class USoundBase;
class USoundAttenuation;
class UTexture2D;
class UAnimationAsset;

class ABaseMagazine;

UCLASS()
class CEPHEDEKICEVHER_API AWeaponAK47 : public AActor ,public IBaseWeaponInterface{
	GENERATED_BODY()
	
	public:
		AWeaponAK47();

		virtual void GunReload() override;
		virtual void SetCollisionContainer() override;
		virtual void SetNoCollision() override;
		virtual UTexture2D *GetWeaponFireModeTexture(uint8 currentMode) override;

		inline virtual void SetCurrentBullet(int16 currentAtilanMermi) override { atilanMermi = currentAtilanMermi; }
		inline virtual void WeaponDestroy() override { this->Destroy(); }
		inline virtual bool GetWeaponIsRifle() override { return true; }
		inline virtual UClass *GetWeaponUClass() override { return this->StaticClass(); }
		inline virtual USkeletalMeshComponent *GetWeaponMesh() override { return WeaponMesh; }
		inline virtual FString GetWeaponObjectName() override { return this->GetName(); }
		inline virtual int16 GetCurrentBullet() override { return atilanMermi; }
		inline virtual int16 GetMaxBullet() override { return maxMermi; }
		inline virtual UClass *GetWeaponBulletClass() override { return bullet; }
		inline virtual UParticleSystem *GetBulletMuzzleParticle() override { return MuzzleParticle; }
		inline virtual USoundBase *WeaponFireSound() override { return ak47FireSound; }
		inline virtual FVector GetWeaponBulletSize() override { return FVector(1, 1, 1); }
		inline virtual USoundAttenuation *WeaponFireSoundAttenuation() override { return ak47SoundAttenuation; }
		inline virtual UTexture2D *GetWeaponTexture() override {return ak47Texture;}
		inline virtual FString GetWeaponName() override { return FString("AK-47"); }
		inline virtual UTexture2D *GetWeaponCrosshair() override { return rifleCrosshair; }
		inline virtual AWeaponAK47 *GetCurrentLookAtWeapon() override { return this;}
		inline virtual USoundBase *GetWeaponEmptyFireSound() override { return ak47EmptySound; }
		inline virtual IBaseWeaponInterface *GetWeaponClass() override { return this; }
		inline virtual USkeletalMeshComponent *GetWeaponHideMesh() override { return WeaponHideMesh; }
		inline virtual UClass *GetWeaponMagazine() override { return MagazineClass; }
		inline virtual UAnimationAsset *GetWeaponFireAnimation() override { return ak47FireAnimation;}
		inline virtual UClass *GetWeaponEjectBulletClass() override { return ejectBullet; }
		inline virtual void SetCurrentWeaponFireMode(uint8 CurrentWeaponFireMode) override { _CurrentWeaponFireMode = CurrentWeaponFireMode; }
		inline virtual uint8 GetCurrentWeaponFireMode() override { return _CurrentWeaponFireMode; }
		virtual void CurrentWeaponEjectBullet() override;

	private:
		uint8 _CurrentWeaponFireMode;
		virtual void BeginPlay() override;
		virtual void Tick(float DeltaTime) override;

		inline virtual bool GetWeaponIsPistol() override { return false; }
		inline virtual bool GetWeaponIsRocketLauncher() override { return false; }

		UPROPERTY(EditDefaultsOnly)
			USkeletalMeshComponent *WeaponMesh;
		
		UPROPERTY(EditDefaultsOnly)
			USkeletalMeshComponent *WeaponHideMesh;

		UPROPERTY(EditDefaultsOnly)
			UCapsuleComponent *CapsuleComponent;

		UPROPERTY(EditDefaultsOnly,Category="Bullet")
			UClass *bullet = ARifleBullet::StaticClass();

		UPROPERTY(EditDefaultsOnly,Category="Bullet")
			UClass *ejectBullet = ARifleFallingBullet::StaticClass();

		UPROPERTY(EditDefaultsOnly,Category="Magazine")
			UClass *MagazineClass = AAK47MagazineItem::StaticClass();
		
		UParticleSystem *MuzzleParticle;
		UTexture2D *rifleCrosshair;
		UTexture2D *ak47Texture;
		UTexture2D *ak47FireModeAuto;
		UTexture2D *ak47FireModeSingle;
		UTexture2D *ak47FireModeSemiAuto;
		int16 atilanMermi;
		int16 kalanMermi;
		int16 sarjordekiMermi;
		int16 maxMermi;
		USoundBase *ak47FireSound;
		USoundBase *ak47EmptySound;
		USoundAttenuation *ak47SoundAttenuation;
		UAnimationAsset *ak47FireAnimation;
};
