#include "GetOldMagNotify.h"

#include "../Soldier/Soldier.h"
#include "../Weapons/BaseWeaponInterface.h"
#include "../CollectibleItems/BaseMagazine.h"

#include "Components/SkeletalMeshComponent.h"
#include "Components/StaticMeshComponent.h"


#include "Kismet/GameplayStatics.h"

#define print(x) GEngine->AddOnScreenDebugMessage(-1,2,FColor::Cyan,FString::Printf(TEXT("%s"),*FString(x)))

void UGetOldMagNotify::Notify(USkeletalMeshComponent *MeshComp, UAnimSequenceBase *Animation){
    Super::Notify(MeshComp, Animation);
    FActorSpawnParameters SpawnParams;
    SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
    if (ASoldier *Soldier = Cast<ASoldier>(MeshComp->GetOwner())){
        const FTransform MagazineTransform(Soldier->GetMesh()->GetSocketRotation("leftHandSocket"), Soldier->GetMesh()->GetSocketLocation("leftHandSocket"), FVector(1, 1, 1));
        ABaseMagazine *Magazine = MeshComp->GetOwner()->GetWorld()->SpawnActor<ABaseMagazine>(Soldier->CurrentRightHandWeapon->GetWeaponMagazine(), MagazineTransform, SpawnParams);
        Magazine->GetCollectibleMesh()->SetHiddenInGame(true);
        Magazine->GetMagazineMesh()->SetHiddenInGame(false);
        switch (Soldier->CurrentWeaponSelected){
            case Soldier->EWeapons::Rifle:
                Soldier->CurrentRightHandWeapon->GetWeaponMesh()->SetHiddenInGame(true);
                Soldier->CurrentRightHandWeapon->GetWeaponHideMesh()->SetHiddenInGame(false);
                if(Magazine){
                    Magazine->AttachToComponent(Soldier->GetMesh(), FAttachmentTransformRules::SnapToTargetIncludingScale, "leftHandSocket");
                    UGameplayStatics::PlaySound2D(MeshComp->GetOwner()->GetWorld(), Magazine->ReloadSound01);
                }
                break;
            
            case Soldier->EWeapons::Pistol:
                if(Magazine){
                    Magazine->AttachToComponent(Soldier->GetMesh(), FAttachmentTransformRules::SnapToTargetIncludingScale, "leftHandSocket");
                    UGameplayStatics::PlaySound2D(MeshComp->GetOwner()->GetWorld(), Magazine->PistolEjectMagSound);
                }
                break;

            case Soldier->EWeapons::RocketLauncher:
            
                break;
        }
    }
    
}