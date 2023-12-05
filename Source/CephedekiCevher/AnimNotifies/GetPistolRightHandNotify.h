#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "GetPistolRightHandNotify.generated.h"

UCLASS()
class CEPHEDEKICEVHER_API UGetPistolRightHandNotify : public UAnimNotify{
	GENERATED_BODY()
	public:
		virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation) override;
};
