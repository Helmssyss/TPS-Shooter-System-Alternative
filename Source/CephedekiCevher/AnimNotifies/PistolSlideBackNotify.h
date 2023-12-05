#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "PistolSlideBackNotify.generated.h"

UCLASS()
class CEPHEDEKICEVHER_API UPistolSlideBackNotify : public UAnimNotify
{
	GENERATED_BODY()
	public:
		virtual void Notify(USkeletalMeshComponent *MeshComp, UAnimSequenceBase *Animation) override;
};
