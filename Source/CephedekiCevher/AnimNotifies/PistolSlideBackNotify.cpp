#include "PistolSlideBackNotify.h"

#include "../Soldier/Soldier.h"
#include "../Weapons/BaseWeaponInterface.h"

#include "Components/SkeletalMeshComponent.h"

void UPistolSlideBackNotify::Notify(USkeletalMeshComponent *MeshComp, UAnimSequenceBase *Animation){
    if(ASoldier *Soldier = Cast<ASoldier>(MeshComp->GetOwner())){
        Soldier->CurrentRightHandWeapon->GetWeaponMesh()->PlayAnimation(Soldier->CurrentRightHandWeapon->GetPistolReloadAnimation(), false);
    }
}