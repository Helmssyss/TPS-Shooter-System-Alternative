#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Components/TimelineComponent.h"
#include "Soldier.generated.h"

class USpringArmComponent;
class UCameraComponent;
class UInputComponent;
class UChildActorComponent;
class USceneComponent;
class UCapsuleComponent;
class UAnimMontage;
class UTimelineComponent;
class USphereComponent;
class USoundBase;
class UCameraShakeBase;

class UWeaponInteractWidget;
class UWeaponCrosshairWidget;
class UWeaponStatusWidget;

class IBaseWeaponInterface;

UCLASS(config=Game)
class ASoldier : public ACharacter{
	GENERATED_BODY()

	public:
		ASoldier();

		UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera)
			USpringArmComponent* CameraBoom;

		UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera)
			UCameraComponent* FollowCamera;
		
		UPROPERTY(EditDefaultsOnly,BlueprintReadOnly)
			UCapsuleComponent *SelectItemCapsule;

		UPROPERTY(EditDefaultsOnly,Category="Soldier Widgets",meta=(AllowPrivateAccess = "true"))
			TSubclassOf<UWeaponInteractWidget> TWeaponInteractWidget;

		UPROPERTY(EditDefaultsOnly,Category="Soldier Widgets",meta=(AllowPrivateAccess = "true"))
			TSubclassOf<UWeaponStatusWidget> TWeaponStatusWidget;

		UPROPERTY()
			UWeaponInteractWidget *WeaponInteractWidget;
		
		UPROPERTY()
			UWeaponStatusWidget *WeaponStatusWidget;

		inline USpringArmComponent *GetCameraBoom() const { return CameraBoom; }
		inline UCameraComponent *GetFollowCamera() const { return FollowCamera; }
		inline void SetReloadState(bool bIsreload) { bReloading = bIsreload; }
		inline bool bGetIsTurnRight() const { return bTurnRight; }
		inline bool bGetIsTurnLeft() const { return bTurnLeft; }
		inline bool bGetIsCrouched() const { return bIsCrouch; }
		inline bool bGetIsProned() const { return bIsProne; }
		inline bool bGetIsReload() const { return bReloading; }
		inline bool bGetIsAimSight() const {return bIsAimSight; }
		inline uint8 GetSoldierHealth() { return SoldierHealth; }
		inline void SetSoldierHealth(uint8 newHealth) { SoldierHealth -= newHealth; }
		inline bool GetSoldierIsDead() { return bSoldierIsDead; }
		void SwapWeapons();
		void SoldierDeath();
		
		uint8 CurrentWeaponSelected;
		IBaseWeaponInterface *CurrentRightHandWeapon;
		IBaseWeaponInterface *CurrentLookAtWeapon;
		USoundBase *WeaponRifleLowerSound;
		USoundBase *WeaponRifleRaiseSound;
		USoundBase *WeaponPistolLowerSound;
		USoundBase *WeaponPistolRaiseSound;
		USoundBase *WeaponRocketLauncherLowerSound;
		USoundBase *WeaponRocketLauncherRaiseSound;
		bool RightHandIsValid;
		bool WeaponIsSwap;
		bool bIsPutWeaponBackPressed;
		bool bSoldierOnFire;
		int16 bullet;
		uint8 CurrentWeaponFireMode;
		float ResultPutWeaponPressedTime;
		TMap<const FString, bool> WeaponsStatusMap;
		enum EWeapons : uint8 {
			Rifle          = 0,
			Pistol         = 1,
			RocketLauncher = 2
		};

	private:
		UPROPERTY(EditDefaultsOnly,Category="Soldier Animation Montages",meta=(AllowPrivateAccess = "true"))
			UAnimMontage *EquipRifleStandingAnimation;

		UPROPERTY(EditDefaultsOnly,Category="Soldier Animation Montages",meta=(AllowPrivateAccess = "true"))
			UAnimMontage *EquipPistolStandingAnimation;

		UPROPERTY(EditDefaultsOnly,Category="Soldier Animation Montages",meta=(AllowPrivateAccess = "true"))
			UAnimMontage *RifleReloadStandingAnimation;
		
		UPROPERTY(EditDefaultsOnly,Category="Soldier Animation Montages",meta=(AllowPrivateAccess = "true"))
			UAnimMontage *FiringRifleAnimation;
		
		UFUNCTION()
			void TimelineProgress(float Value);
		UFUNCTION()
			void BodyHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);
		UFUNCTION()
			void SelectItemBeginOverlap(UPrimitiveComponent *OverlappedComponent, AActor *OtherActor, UPrimitiveComponent *OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult &SweepResult);
		UFUNCTION()
			void SelectItemEndOverlap(UPrimitiveComponent *OverlappedComponent, AActor *OtherActor, UPrimitiveComponent *OtherComp, int32 OtherBodyIndex);
		virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;
		virtual void BeginPlay() override;
		virtual void EndPlay(EEndPlayReason::Type EndPlayReason) override;
		virtual void Tick(float DeltaTime) override;
		void MoveForward(float Value);
		void MoveRight(float Value);
		void Sprint();
		void StopSprint();
		void Turn(float Rate);
		void LookUp(float Rate);
		void SoldierCrouch();
		void SoldierProne();
		void EquipWeapons();
		void PrimaryWeapon();
		void SecondaryWeapon();
		void ThirdyWeapon();
		void DropWeapon();
		void PutWeaponBackPressed();
		void WeaponReload();
		void PutWeaponBackEnd();
		void PutWeaponIsReleasedLoop();
		void PutWeaponBackCheck();
		void Fire();
		void AimSight();
		void AimDownSight();
		void LookAroundHold();
		void LookAroundReleased();
		void SetFireMode();
		void SetFireModeClear();
		void FireMode();
		void SemiAutoFire();
		void SoldierRespawn();
		void TickTimeline();
		FVector SelectVector(const FVector A, const FVector B, const bool bSelectA) const;
		FRotator FindLookAtRotation(const FVector &Start, const FVector &Target);
		bool LineTraceCreate(const int16 &traceDistance = 500, const float &Thickness = 1.0f, const FColor &lineColor = FColor::Red);

		FTimerHandle PutWeaponIsReleasedLoopHandle;
		FTimerHandle ReloadTimerHandle;
		FTimerHandle TH_RapidFaireHandle;
		FTimerHandle TimerHandle;
		FTimerHandle RespawnHandle;
		FTimeline TimeLine;
		FHitResult LineHitWeapon;
		UCurveFloat *FloatCurve;
		TSubclassOf<UCameraShakeBase> CameraShake;
		USoundBase *FireModeChangeSound;
		bool bTurnRight;
		bool bTurnLeft;
		bool bIsCrouch;
		bool bIsSprint;
		bool bIsProne;
		bool bPutWeaponIsReleased;
		bool bWeaponSelected;
		bool bReloading;
		bool bIsAimSight;
		bool bSoldierIsDead;
		uint8 PutWeaponPressedTime;
		uint8 SemiAutoFireCount;
		uint8 SoldierHealth = 100;
		float BaseTurnRate;
		float BaseLookUpRate;
		float DELTATIME = 0.1f;
};