#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "WeaponInteractWidget.generated.h"

class UImage;
class UTextBlock;
class UTexture2D;

class ASoldier;

UCLASS()
class CEPHEDEKICEVHER_API UWeaponInteractWidget : public UUserWidget{
	GENERATED_BODY()
	
	public:
		virtual void NativeConstruct() override;
		// virtual bool Initialize() override;

		UPROPERTY(meta=(BindWidget))
			UImage* WeaponImage;
		
		UPROPERTY(meta=(BindWidget))
			UTextBlock* WeaponText;
		
		UPROPERTY(meta=(BindWidget))
			UTextBlock *WeaponCurrentBulletText;

	private:
		UTexture2D *crossHair;
		// UFUNCTION()
		// 	FText GetWeaponTextName();
		ASoldier* Soldier;
};
