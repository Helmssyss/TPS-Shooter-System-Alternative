#pragma once

#include "CoreMinimal.h"
#include "BaseMagazine.h"
#include "AK47MagazineItem.generated.h"

class UStaticMeshComponent;

UCLASS()
class CEPHEDEKICEVHER_API AAK47MagazineItem : public ABaseMagazine{
	GENERATED_BODY()
	
	public:
		AAK47MagazineItem();

		inline virtual UStaticMeshComponent *GetMagazineMesh() override { return MagazineMesh; }
		inline virtual UStaticMeshComponent *GetCollectibleMesh() override { return CollectibleMesh; }

	private:
		virtual void BeginPlay() override;
		virtual void Tick(float DeltaTime) override;

		UStaticMeshComponent* MagazineMesh;
		UAnimationAsset *ReloadAnimation;
};
