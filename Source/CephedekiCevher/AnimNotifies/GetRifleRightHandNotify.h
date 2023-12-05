#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "GetRifleRightHandNotify.generated.h"

UCLASS()
class CEPHEDEKICEVHER_API UGetRifleRightHandNotify : public UAnimNotify{
	GENERATED_BODY()
	
	public:
		virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation) override;
	
};
