// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseMagazine.h"
#include "AR4MagazineItem.generated.h"

/**
 * 
 */
UCLASS()
class CEPHEDEKICEVHER_API AAR4MagazineItem : public ABaseMagazine
{
	GENERATED_BODY()

	public:
		AAR4MagazineItem();
		
		inline virtual UStaticMeshComponent *GetMagazineMesh() override { return MagazineMesh; }
		inline virtual UStaticMeshComponent *GetCollectibleMesh() override { return CollectibleMesh;}

	private:
		virtual void BeginPlay() override;
		virtual void Tick(float DeltaTime) override;

		UStaticMeshComponent *MagazineMesh;
};
