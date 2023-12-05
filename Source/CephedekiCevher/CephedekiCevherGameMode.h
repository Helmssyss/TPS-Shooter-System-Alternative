#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "CephedekiCevherGameMode.generated.h"

class ASoldier;

UCLASS(minimalapi)
class ACephedekiCevherGameMode : public AGameModeBase{
	GENERATED_BODY()
	
	public:
		ACephedekiCevherGameMode();
		
    	void RespawnPlayer(ASoldier* PlayerToRespawn);
};