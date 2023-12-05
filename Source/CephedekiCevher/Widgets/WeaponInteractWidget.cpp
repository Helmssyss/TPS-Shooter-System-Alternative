#include "WeaponInteractWidget.h"
#include "../Soldier/Soldier.h"
#include "../Weapons/BaseWeaponInterface.h"

#include "Components/TextBlock.h"
#include "Components/Image.h"

#define print(x) GEngine->AddOnScreenDebugMessage(-1,2,FColor::Cyan,FString::Printf(TEXT("%s"),*FString(x)))

void UWeaponInteractWidget::NativeConstruct(){
    Super::NativeConstruct();
    TArray<FStringFormatArg> args;
    Soldier = Cast<ASoldier>(GetOwningPlayerPawn());
    if(Soldier){
        if(Soldier->CurrentLookAtWeapon){
            FSlateBrush Brush = WeaponImage->Brush;
            WeaponText->SetText(FText::FromString(Soldier->CurrentLookAtWeapon->GetWeaponName()));
            args.Add(FStringFormatArg(Soldier->CurrentLookAtWeapon->GetCurrentBullet()));
            args.Add(FStringFormatArg(Soldier->CurrentLookAtWeapon->GetMaxBullet()));
            WeaponCurrentBulletText->SetText(FText::FromString(FString::Format(TEXT("{0}/{1}"), args)));
            Brush.SetResourceObject(Soldier->CurrentLookAtWeapon->GetWeaponTexture());
            WeaponImage->SetBrush(Brush);
        }
    }
}

// bool UWeaponInteractWidget::Initialize(){
//     bool initialize = Super::Initialize();

//     if(!initialize)
//         return false;
//     if(WeaponName){
//         WeaponName->TextDelegate.BindUFunction(this, "GetWeaponTextName");
//     }
//     return true;
// }

// FText UWeaponInteractWidget::GetWeaponTextName(){
//     return FText::FromString(WeaponItems.WeaponName);
// }