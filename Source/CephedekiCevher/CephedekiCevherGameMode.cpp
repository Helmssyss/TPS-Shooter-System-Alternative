#include "CephedekiCevherGameMode.h"
#include "Soldier/Soldier.h"
#include "UObject/ConstructorHelpers.h"

ACephedekiCevherGameMode::ACephedekiCevherGameMode(){
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/_Blueprints/BP_Soldier"));
	if (PlayerPawnBPClass.Class != NULL){
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}

void ACephedekiCevherGameMode::RespawnPlayer(ASoldier *PlayerToRespawn){
	if (PlayerToRespawn){
		PlayerToRespawn->Destroy();
		FActorSpawnParameters SpawnParams;
		SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButDontSpawnIfColliding;
		FVector SpawnLocation = FVector(-2170.630371, -365.506042, 222.000671);
		FRotator SpawnRotation = FRotator::ZeroRotator;
		ASoldier *SoldierSpawned = GetWorld()->SpawnActor<ASoldier>(DefaultPawnClass, FTransform(SpawnRotation, SpawnLocation), SpawnParams);
		if (SoldierSpawned){
			APlayerController* PlayerController = GetWorld()->GetFirstPlayerController();
			if (PlayerController){
				PlayerController->Possess(SoldierSpawned);
			}
		}
	}
}