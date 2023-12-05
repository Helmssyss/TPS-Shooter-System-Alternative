#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "SoldierAnimation.generated.h"

class ASoldier;

UCLASS()
class CEPHEDEKICEVHER_API USoldierAnimation : public UAnimInstance
{
	GENERATED_BODY()

	private:
		virtual void NativeUpdateAnimation(float DeltaSeconds) override;

		FRotator GetLookRotation(ASoldier *soldier);
		FRotator InverseTransformRotation(const FTransform &Transform, FRotator Rotation);

		UPROPERTY(BlueprintReadonly, Category = "Soldier", meta = (AllowPrivateAccess = "true") )
			float SoldierDirection;
			
		UPROPERTY(BlueprintReadonly, Category = "Soldier", meta = (AllowPrivateAccess = "true") )
			float SoldierSpeed;
			
		UPROPERTY(BlueprintReadonly, Category = "Soldier", meta = (AllowPrivateAccess = "true") )
			float characterRotCoordinatePITCH;

		UPROPERTY(BlueprintReadonly, Category = "Soldier", meta = (AllowPrivateAccess = "true") )
			float characterRotCoordinateYAW;
		
		UPROPERTY(BlueprintReadonly, Category = "Soldier", meta = (AllowPrivateAccess = "true") )
			bool bSoldierTurnLeft;

		UPROPERTY(BlueprintReadonly, Category = "Soldier", meta = (AllowPrivateAccess = "true") )
			bool bSoldierTurnRight;

		UPROPERTY(BlueprintReadonly, Category = "Soldier", meta = (AllowPrivateAccess = "true") )
			bool bSoldierCrouch;

		UPROPERTY(BlueprintReadonly, Category = "Soldier", meta = (AllowPrivateAccess = "true") )
			bool bSoldierIsGround = !bSoldierCrouch;
			
		UPROPERTY(BlueprintReadonly, Category = "Soldier", meta = (AllowPrivateAccess = "true") )
			bool bSoldierIsAir;

		UPROPERTY(BlueprintReadonly, Category = "Soldier", meta = (AllowPrivateAccess = "true") )
			bool bSoldierRightHandIsValid;
		
		UPROPERTY(BlueprintReadonly, Category = "Soldier", meta = (AllowPrivateAccess = "true") )
			bool bSoldierProne;

		UPROPERTY(BlueprintReadonly, Category = "Soldier", meta = (AllowPrivateAccess = "true") )
			bool bSoldierReload;
		
		UPROPERTY(BlueprintReadonly, Category = "Soldier", meta = (AllowPrivateAccess = "true") )
			bool bSoldierIsAimSight;

		UPROPERTY(BlueprintReadonly, Category = "Soldier", meta = (AllowPrivateAccess = "true") )
			bool bSoldierInRightHandWeaponIsPistol;

		UPROPERTY(BlueprintReadonly, Category = "Soldier", meta = (AllowPrivateAccess = "true") )
			bool bSoldierInRightHandWeaponIsRifle;

		UPROPERTY(BlueprintReadonly, Category = "Soldier", meta = (AllowPrivateAccess = "true") )
			bool bSoldierInRightHandWeaponIsRocketLauncher;

		UPROPERTY(BlueprintReadonly, Category = "Soldier", meta = (AllowPrivateAccess = "true") )
			uint8 SoldierWeaponSelected;

		UPROPERTY(BlueprintReadonly, Category = "Soldier Rotation", meta = (AllowPrivateAccess = "true") )
			FRotator SoldierHeadRotation;
};
