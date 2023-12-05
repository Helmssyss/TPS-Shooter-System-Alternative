#pragma once

#include "CoreMinimal.h"
#include "BaseMagazine.h"
#include "PistolMagazineItem.generated.h"

class UStaticMeshComponent;

UCLASS()
class CEPHEDEKICEVHER_API APistolMagazineItem : public ABaseMagazine{
	GENERATED_BODY()

	public:
		APistolMagazineItem();

		inline virtual UStaticMeshComponent *GetMagazineMesh() override { return MagazineMesh; }
		inline virtual UStaticMeshComponent *GetCollectibleMesh() { return CollectibleMesh; }

	private:
		virtual void BeginPlay() override;
		
		UStaticMeshComponent* MagazineMesh;
		UAnimationAsset *ReloadAnimation;
};
