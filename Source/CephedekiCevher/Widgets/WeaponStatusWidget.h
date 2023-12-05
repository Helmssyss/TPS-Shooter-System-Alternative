#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "WeaponStatusWidget.generated.h"

class UImage;
class UTextBlock;
class ASoldier;

UCLASS()
class CEPHEDEKICEVHER_API UWeaponStatusWidget : public UUserWidget{
	GENERATED_BODY()
	public:
		virtual void NativeConstruct() override;
		virtual bool Initialize() override;

		UPROPERTY(meta=(BindWidget))
			UImage *WeaponImage;
		
		UPROPERTY(meta=(BindWidget))
			UImage *WeaponCrosshair;

		UPROPERTY(meta=(BindWidget))
			UImage *FireModeImage;

		UPROPERTY(meta=(BindWidget))
			UTextBlock *WeaponBulletCount;
		
		UPROPERTY(meta=(BindWidget))
			UTextBlock *WeaponName;

		UPROPERTY(meta=(BindWidget))
			UTextBlock *WeaponMaxBulletCount;

	private:
		UFUNCTION()
			FSlateBrush FBWeaponImage();

		UFUNCTION()
			FSlateBrush FTFireModeImage();

		UFUNCTION()
			FText FTWeaponBulletCount();
		
		UFUNCTION()
			FText FTWeaponMaxBulletCount();

		UFUNCTION()
			FText FTWeaponName();
		
		UFUNCTION()
			FSlateBrush FBWeaponCrosshairImage();
			
		ASoldier *Soldier;
};
