#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "GetNewMagNotify.generated.h"

UCLASS()
class CEPHEDEKICEVHER_API UGetNewMagNotify : public UAnimNotify
{
	GENERATED_BODY()
	public:
		virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation) override;
};
