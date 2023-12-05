#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "ReleasedMagNotify.generated.h"

UCLASS()
class CEPHEDEKICEVHER_API UReleasedMagNotify : public UAnimNotify
{
	GENERATED_BODY()
	public:
		virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation) override;
};
