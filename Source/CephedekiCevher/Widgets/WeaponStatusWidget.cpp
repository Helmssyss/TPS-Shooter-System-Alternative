#include "WeaponStatusWidget.h"
#include "../Soldier/Soldier.h"
#include "../Weapons/BaseWeaponInterface.h"

#include "Components/TextBlock.h"
#include "Components/Image.h"

#define print(x) GEngine->AddOnScreenDebugMessage(-1,2,FColor::Cyan,FString::Printf(TEXT("%s"),*FString(x)))

void UWeaponStatusWidget::NativeConstruct(){
    Super::NativeConstruct();
    Soldier = Cast<ASoldier>(GetOwningPlayerPawn());
}

bool UWeaponStatusWidget::Initialize(){
    bool initialize = Super::Initialize();
    WeaponImage->BrushDelegate.BindUFunction(this, "FBWeaponImage");
    WeaponCrosshair->BrushDelegate.BindUFunction(this, "FBWeaponCrosshairImage");
    WeaponBulletCount->TextDelegate.BindUFunction(this, "FTWeaponBulletCount");
    WeaponName->TextDelegate.BindUFunction(this, "FTWeaponName");
    WeaponMaxBulletCount->TextDelegate.BindUFunction(this, "FTWeaponMaxBulletCount");
    FireModeImage->BrushDelegate.BindUFunction(this, "FTFireModeImage");

    if(!initialize)
        return false;

    return true;
}

FSlateBrush UWeaponStatusWidget::FBWeaponImage(){
    if(Soldier && Soldier->CurrentRightHandWeapon){
        FSlateBrush Brush = WeaponImage->Brush;
        Brush.SetResourceObject(Soldier->CurrentRightHandWeapon->GetWeaponTexture());
        WeaponImage->SetBrush(Brush);
        WeaponImage->SetVisibility(ESlateVisibility::Visible);
        return Brush;
    }
    FSlateBrush _;
    return _;
}

FSlateBrush UWeaponStatusWidget::FTFireModeImage(){
    if(Soldier){
        FSlateBrush Brush = FireModeImage->Brush;
        Brush.SetResourceObject(Soldier->CurrentRightHandWeapon->GetWeaponFireModeTexture(Soldier->CurrentRightHandWeapon->GetCurrentWeaponFireMode()));
        FireModeImage->SetBrush(Brush);
        return Brush;
    }
    FSlateBrush _;
    return _;
}

FSlateBrush UWeaponStatusWidget::FBWeaponCrosshairImage(){
    if(Soldier && Soldier->CurrentRightHandWeapon){
        FSlateBrush Brush = WeaponCrosshair->Brush;
        Brush.SetResourceObject(Soldier->CurrentRightHandWeapon->GetWeaponCrosshair());
        WeaponCrosshair->SetBrush(Brush);
        return Brush;
    }
    FSlateBrush _;
    return _;
}

FText UWeaponStatusWidget::FTWeaponBulletCount(){
    if(Soldier->CurrentRightHandWeapon)
        return FText::FromString(FString::FromInt(Soldier->CurrentRightHandWeapon->GetCurrentBullet()));
    else
        return FText::GetEmpty();
}

FText UWeaponStatusWidget::FTWeaponMaxBulletCount(){
    if(Soldier->CurrentRightHandWeapon)
        return FText::FromString(FString::FromInt(Soldier->CurrentRightHandWeapon->GetMaxBullet()));
    else
        return FText::GetEmpty();
}

FText UWeaponStatusWidget::FTWeaponName(){
    if(Soldier->CurrentRightHandWeapon)
        return FText::FromString(Soldier->CurrentRightHandWeapon->GetWeaponName());
    else
        return FText::GetEmpty();
}