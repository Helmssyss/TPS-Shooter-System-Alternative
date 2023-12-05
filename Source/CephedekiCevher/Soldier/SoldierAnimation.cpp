#include "SoldierAnimation.h"
#include "Soldier.h"
#include "../Weapons/BaseWeaponInterface.h"

#include "GameFramework/PawnMovementComponent.h"

void USoldierAnimation::NativeUpdateAnimation(float DeltaSeconds){
    Super::NativeUpdateAnimation(DeltaSeconds);

    FVector Velocity;
    if(ASoldier *TryPawn = Cast<ASoldier>(TryGetPawnOwner())){
        SoldierWeaponSelected = TryPawn->CurrentWeaponSelected;

        bSoldierRightHandIsValid = TryPawn->RightHandIsValid;
        bSoldierReload = TryPawn->bGetIsReload();
        bSoldierProne = TryPawn->bGetIsProned();
        bSoldierTurnLeft = TryPawn->bGetIsTurnLeft();
        bSoldierTurnRight = TryPawn->bGetIsTurnRight();
        bSoldierCrouch = TryPawn->bGetIsCrouched();
        bSoldierIsAir = TryPawn->GetMovementComponent()->IsFalling();
        bSoldierIsAimSight = TryPawn->bGetIsAimSight();
        if(TryPawn->CurrentRightHandWeapon){
            bSoldierInRightHandWeaponIsPistol = TryPawn->CurrentRightHandWeapon->GetWeaponIsPistol();
            bSoldierInRightHandWeaponIsRifle = TryPawn->CurrentRightHandWeapon->GetWeaponIsRifle();
            bSoldierInRightHandWeaponIsRocketLauncher = TryPawn->CurrentRightHandWeapon->GetWeaponIsRocketLauncher();
        }

        SoldierSpeed = TryPawn->GetVelocity().Size();

        SoldierDirection = CalculateDirection(TryPawn->GetVelocity(), TryPawn->GetActorRotation());
        GetLookRotation(TryPawn);
    }
}

FRotator USoldierAnimation::GetLookRotation(ASoldier* soldier){
    const FTransform pawnTransform = soldier->GetActorTransform();
    FRotator pawnControlRotation = soldier->GetControlRotation();
    FRotator inverseTransform = InverseTransformRotation(pawnTransform, pawnControlRotation);
    FRotator makeRotator = FRotator(0.0f, FMath::ClampAngle(inverseTransform.Yaw, -70.0f, 70.0f), FMath::ClampAngle((inverseTransform.Pitch * -1.0f), -70.0f, 70.0f));
    if(FMath::Abs(inverseTransform.Yaw ) > 170.0f){
        SoldierHeadRotation = FRotator::ZeroRotator;
        // soldier->EndFire();
    }else
        SoldierHeadRotation = FMath::RInterpTo(SoldierHeadRotation, makeRotator, GetWorld()->GetDeltaSeconds(), 9.0f);
    return SoldierHeadRotation;
}

FRotator USoldierAnimation::InverseTransformRotation(const FTransform &Transform, FRotator Rotation){
    return Transform.InverseTransformRotation(Rotation.Quaternion()).Rotator();
}