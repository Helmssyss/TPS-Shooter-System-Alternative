// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "BulletHitWidget.generated.h"

/**
 * 
 */
UCLASS()
class CEPHEDEKICEVHER_API UBulletHitWidget : public UUserWidget
{
	GENERATED_BODY()

	public:
		virtual void NativeConstruct() override;
		
		UPROPERTY(BlueprintReadOnly,Transient,meta=(BindWidgetAnim))
			UWidgetAnimation *hitOpacity;
};
