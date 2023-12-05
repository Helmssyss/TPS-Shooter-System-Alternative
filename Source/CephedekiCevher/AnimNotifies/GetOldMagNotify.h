#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "GetOldMagNotify.generated.h"

UCLASS()
class CEPHEDEKICEVHER_API UGetOldMagNotify : public UAnimNotify{
	GENERATED_BODY()
	public:
		virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation) override;
};
