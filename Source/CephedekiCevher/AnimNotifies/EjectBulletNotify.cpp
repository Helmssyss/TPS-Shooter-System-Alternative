#include "EjectBulletNotify.h"


#include "../Weapons/BaseWeaponInterface.h"

#include "Components/SkeletalMeshComponent.h"

#define print(x) GEngine->AddOnScreenDebugMessage(-1,2,FColor::Cyan,FString::Printf(TEXT("%s"),*FString(x)))

void UEjectBulletNotify::Notify(USkeletalMeshComponent *MeshComp, UAnimSequenceBase *Animation){
    Super::Notify(MeshComp, Animation);

    if(IBaseWeaponInterface *Weapon = Cast<IBaseWeaponInterface>(MeshComp->GetOwner())){
        Weapon->CurrentWeaponEjectBullet();
    }
}