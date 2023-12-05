#include "ThrowMagNotify.h"

#include "../Soldier/Soldier.h"
#include "../Weapons/BaseWeaponInterface.h"
#include "../CollectibleItems/BaseMagazine.h"

#include "Components/SkeletalMeshComponent.h"
#include "Components/StaticMeshComponent.h"


#include "Kismet/GameplayStatics.h"

void UThrowMagNotify::Notify(USkeletalMeshComponent *MeshComp, UAnimSequenceBase *Animation){
    Super::Notify(MeshComp, Animation);
    FActorSpawnParameters SpawnParams;
    SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
    if (ASoldier *Soldier = Cast<ASoldier>(MeshComp->GetOwner())){
        TArray<USceneComponent *> childrenComponents;
        switch (Soldier->CurrentWeaponSelected){
            case Soldier->EWeapons::Rifle:
                Soldier->GetMesh()->GetChildrenComponents(false, childrenComponents);
                for(USceneComponent *i : childrenComponents){
                    ABaseMagazine *Magazine = Cast<ABaseMagazine>(i->GetOwner());
                    if(Magazine){
                        Magazine->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
                        Magazine->GetMagazineMesh()->SetSimulatePhysics(true);
                        Magazine->GetMagazineMesh()->SetCollisionEnabled(ECollisionEnabled::PhysicsOnly);
                        Magazine->SetLifeSpan(3);
                    }
                }
                    // UGameplayStatics::PlaySound2D(MeshComp->GetOwner()->GetWorld(), Magazine->ReloadSound02);
                break;
            
            case Soldier->EWeapons::Pistol:
                Soldier->GetMesh()->GetChildrenComponents(false, childrenComponents);
                for(USceneComponent *i : childrenComponents){
                    ABaseMagazine *Magazine = Cast<ABaseMagazine>(i->GetOwner());
                    if(Magazine){
                        Magazine->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
                        Magazine->GetMagazineMesh()->SetSimulatePhysics(true);
                        Magazine->GetMagazineMesh()->SetCollisionEnabled(ECollisionEnabled::PhysicsOnly);
                        Magazine->SetLifeSpan(3);
                    }
                }
                break;

            // case Soldier->EWeapons::RocketLauncher:
            //     break;
        }
    }
}