#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BaseMagazine.generated.h"

class UStaticMeshComponent;
class USoundBase;
class UAnimationAsset;

UCLASS()
class CEPHEDEKICEVHER_API ABaseMagazine : public AActor{
	GENERATED_BODY()
	
	public:
		ABaseMagazine();

		virtual UStaticMeshComponent* GetMagazineMesh() {return nullptr;}
		virtual UStaticMeshComponent *GetCollectibleMesh() { return nullptr; }

		USoundBase *ReloadSound01;
		USoundBase *ReloadSound02;
		USoundBase *ReloadSound03;

		USoundBase *PistolEjectMagSound;
		USoundBase *PistolReloadSlideSound;

	protected:
		UStaticMeshComponent *CollectibleMesh;
};
