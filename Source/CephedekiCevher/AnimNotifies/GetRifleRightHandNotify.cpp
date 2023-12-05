#include "GetRifleRightHandNotify.h"

#include "../Soldier/Soldier.h"
#include "../Weapons/BaseWeaponInterface.h"
#include "../Widgets/WeaponStatusWidget.h"

#include "Components/SkeletalMeshComponent.h"

#include "Kismet/GameplayStatics.h"

#define print(x) GEngine->AddOnScreenDebugMessage(-1,2,FColor::Cyan,FString::Printf(TEXT("%s"),*FString(x)))

void UGetRifleRightHandNotify::Notify(USkeletalMeshComponent *MeshComp, UAnimSequenceBase *Animation){
    Super::Notify(MeshComp, Animation);

    if (ASoldier *Soldier = Cast<ASoldier>(MeshComp->GetOwner())){
        const uint8 WeaponSelect = Soldier->CurrentWeaponSelected;
        
        if(WeaponSelect == 0){
            Soldier->RightHandIsValid = true;
            if(Soldier->WeaponIsSwap){
                Soldier->SwapWeapons();
                if(!Soldier->WeaponStatusWidget->IsInViewport()){
                    Soldier->WeaponStatusWidget->AddToViewport();
                }else{
                    Soldier->WeaponStatusWidget->RemoveFromParent();
                }
            }
            TArray<USceneComponent *> SoldierGetAttachedActors;
            Soldier->GetMesh()->GetChildrenComponents(false, SoldierGetAttachedActors);
            for(USceneComponent* i : SoldierGetAttachedActors){
                IBaseWeaponInterface *Weapon = Cast<IBaseWeaponInterface>(i->GetOwner());
                if(Weapon!=nullptr){
                    if(Weapon->GetWeaponIsRifle()){
                        Soldier->CurrentRightHandWeapon = Weapon;
                        Soldier->bullet = Soldier->CurrentRightHandWeapon->GetCurrentBullet();
                        Soldier->CurrentWeaponFireMode = Soldier->CurrentRightHandWeapon->GetCurrentWeaponFireMode();
                        Soldier->SetReloadState(false);
                        UGameplayStatics::PlaySound2D(MeshComp->GetOwner()->GetWorld(), Soldier->WeaponRifleLowerSound);
                        i->GetOwner()->AttachToComponent(Soldier->GetMesh(), FAttachmentTransformRules::SnapToTargetIncludingScale,"rightHandSocket");
                        if(Soldier->ResultPutWeaponPressedTime >= 1.3f){
                            print("sa");
                            Soldier->bIsPutWeaponBackPressed = true;
                            Soldier->RightHandIsValid = false;
                            Soldier->ResultPutWeaponPressedTime = 0;
                            i->GetOwner()->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
                            i->GetOwner()->AttachToComponent(Soldier->GetMesh(), FAttachmentTransformRules::SnapToTargetIncludingScale,"rightBackSocket");
                        }
                    }
                }
            }

            if(!Soldier->WeaponStatusWidget->IsInViewport()){
                Soldier->WeaponStatusWidget->AddToViewport();
            }
            if(Soldier->bIsPutWeaponBackPressed){
                Soldier->WeaponStatusWidget->RemoveFromParent();
                Soldier->bIsPutWeaponBackPressed = false;
            }

        }else if(WeaponSelect == 2){
            Soldier->RightHandIsValid = true;
            if(Soldier->WeaponIsSwap){
                Soldier->SwapWeapons();
                if(!Soldier->WeaponStatusWidget->IsInViewport()){
                    Soldier->WeaponStatusWidget->AddToViewport();
                }else{
                    Soldier->WeaponStatusWidget->RemoveFromParent();
                }
            }
            TArray<USceneComponent *> SoldierGetAttachedActors;
            Soldier->GetMesh()->GetChildrenComponents(false, SoldierGetAttachedActors);
            for(USceneComponent* i : SoldierGetAttachedActors){
                IBaseWeaponInterface *Weapon = Cast<IBaseWeaponInterface>(i->GetOwner());
                if(Weapon!=nullptr){
                    if(Weapon->GetWeaponIsRocketLauncher()){
                        Soldier->CurrentRightHandWeapon = Weapon;
                        Soldier->bullet = Soldier->CurrentRightHandWeapon->GetCurrentBullet();
                        Soldier->CurrentWeaponFireMode = Soldier->CurrentRightHandWeapon->GetCurrentWeaponFireMode();
                        Soldier->SetReloadState(false);
                        UGameplayStatics::PlaySound2D(MeshComp->GetOwner()->GetWorld(), Soldier->WeaponRocketLauncherLowerSound);
                        i->GetOwner()->AttachToComponent(Soldier->GetMesh(), FAttachmentTransformRules::SnapToTargetIncludingScale,"rightHandSocket");
                        if(Soldier->ResultPutWeaponPressedTime >= 1.3f){
                            Soldier->bIsPutWeaponBackPressed = true;
                            Soldier->RightHandIsValid = false;
                            Soldier->ResultPutWeaponPressedTime = 0;
                            i->GetOwner()->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
                            i->GetOwner()->AttachToComponent(Soldier->GetMesh(), FAttachmentTransformRules::SnapToTargetIncludingScale,"rocketLaunchBackSocket");
                        }
                    }
                }
            }

            if(!Soldier->WeaponStatusWidget->IsInViewport()){
                    Soldier->WeaponStatusWidget->AddToViewport();
            }
            if(Soldier->bIsPutWeaponBackPressed){
                Soldier->WeaponStatusWidget->RemoveFromParent();
                Soldier->bIsPutWeaponBackPressed = false;
            }
        }
    }
}