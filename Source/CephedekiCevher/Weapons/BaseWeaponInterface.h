#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "UObject/ConstructorHelpers.h"
#include "BaseWeaponInterface.generated.h"

class USkeletalMeshComponent;
class UTexture2D;
class UParticleSystem;
class USoundBase;
class USoundAttenuation;
class UAnimationAsset;

class ABaseMagazine;

UINTERFACE(MinimalAPI)
class UBaseWeaponInterface : public UInterface{GENERATED_BODY()};

class CEPHEDEKICEVHER_API IBaseWeaponInterface{
	GENERATED_BODY()

	public:
		virtual void GunReload(void) = 0;
		virtual void WeaponDestroy(void) = 0;
		virtual void SetCollisionContainer(void) = 0;
		virtual void SetNoCollision(void) = 0;
		virtual void SetCurrentBullet(int16 currentAtilanMermi) = 0;
		virtual void SetCurrentWeaponFireMode(uint8 CurrentWeaponFireMode) = 0;
		virtual bool GetWeaponIsRifle(void) = 0;
		virtual bool GetWeaponIsPistol(void) = 0;
		virtual bool GetWeaponIsRocketLauncher(void) = 0;
		virtual USkeletalMeshComponent *GetWeaponMesh(void) = 0;
		virtual FString GetWeaponObjectName(void) = 0;
		virtual int16 GetCurrentBullet(void) = 0;
		virtual int16 GetMaxBullet(void) = 0;
		virtual UParticleSystem *GetBulletMuzzleParticle(void) = 0;
		virtual USoundBase *WeaponFireSound(void) = 0;
		virtual USoundAttenuation *WeaponFireSoundAttenuation(void) = 0;
		virtual FVector GetWeaponBulletSize(void) = 0;
		virtual UTexture2D *GetWeaponTexture(void) = 0;
		virtual FString GetWeaponName(void) = 0;
		virtual UTexture2D *GetWeaponCrosshair(void) = 0;
		virtual IBaseWeaponInterface *GetCurrentLookAtWeapon(void) = 0;
		virtual IBaseWeaponInterface *GetWeaponClass(void) = 0;
		virtual USoundBase *GetWeaponEmptyFireSound(void) = 0;
		virtual USkeletalMeshComponent *GetWeaponHideMesh(void) = 0;
		virtual UClass *GetWeaponMagazine(void) = 0;
		virtual UClass *GetWeaponUClass(void) = 0;
		virtual UClass *GetWeaponBulletClass(void) = 0;
		virtual UClass *GetWeaponEjectBulletClass(void) = 0;
		virtual UAnimationAsset *GetWeaponFireAnimation(void) = 0;
		virtual UAnimationAsset *GetPistolReloadAnimation(void) { return nullptr; }
		virtual uint8 GetCurrentWeaponFireMode(void) = 0;
		virtual UTexture2D *GetWeaponFireModeTexture(uint8 currentMode) = 0;
		virtual void CurrentWeaponEjectBullet(void){}
		virtual UParticleSystem *GetRocketLauncherMuzzleRearParticle(void) { return nullptr; }
};

enum EWeaponFireMode : uint8{
	Auto,
	Single,
	SemiAuto
};