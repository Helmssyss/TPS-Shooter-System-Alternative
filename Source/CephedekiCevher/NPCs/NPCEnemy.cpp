#include "NPCEnemy.h"
#include "../Bullets/BaseBullet.h"

#include "Components/SkeletalMeshComponent.h"
#include "Components/CapsuleComponent.h"

#include "UObject/ConstructorHelpers.h"
#include "TimerManager.h"

#include "AIController.h"

#define print(x) GEngine->AddOnScreenDebugMessage(-1,2,FColor::Cyan,FString::Printf(TEXT("%s"),*FString(x)))

ANPCEnemy::ANPCEnemy(){
	GetCapsuleComponent()->InitCapsuleSize(12.005492, 74.917404);
	GetMesh()->SetRelativeLocation(FVector(-0.000114, 0.000614, -77.549782));
	GetMesh()->SetRelativeRotation(FRotator(0.0, -90.0, 0.0));
	PrimaryActorTick.bCanEverTick = false;
	bUseControllerRotationYaw = true;

	static ConstructorHelpers::FObjectFinder<USkeletalMesh> GetEnemyNPCSkeletalMesh(TEXT("/Game/_NPCsContent/_NPCsMeshes/EnemyCharacter"));

	if(GetEnemyNPCSkeletalMesh.Succeeded()){
		GetMesh()->SetSkeletalMesh(GetEnemyNPCSkeletalMesh.Object);
	}
	SetNPCsCollisionContainer();
}

void ANPCEnemy::BeginPlay(){
	Super::BeginPlay();
	GetMesh()->OnComponentHit.AddDynamic(this, &ANPCEnemy::BodyHit);
	AAIController *GetAIController = Cast<AAIController>(GetController());
	if(GetAIController){
		GetAIController->Possess(this);
		print("AI_Controller Var");
		GetAIController->MoveToLocation(MoveToTargetLocation);
	}else{
		print("AI_Controller yok");
	}
}

void ANPCEnemy::Tick(float DeltaTime){
	Super::Tick(DeltaTime);
}

void ANPCEnemy::DeathEnemy() {
    AController* C_Controller = GetController();
    if (C_Controller) {
        C_Controller->UnPossess();
		C_Controller->Destroy();
	}
    GetCapsuleComponent()->DestroyComponent();
    GetMesh()->SetAllBodiesBelowSimulatePhysics(FName("Hips"), true);
    bEnemyIsDead = true;
    Health = 0;
    GetWorld()->GetTimerManager().SetTimer(RespawnHandle, this, &ANPCEnemy::EnemyRespawn, 5.f, false);
}

void ANPCEnemy::EnemyRespawn() {
    FVector RespawnLocation = FVector(-2416.051758, -293.712067, 265.626831);
    FActorSpawnParameters SpawnParams;
    SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
    ANPCEnemy* SpawnedEnemy = GetWorld()->SpawnActor<ANPCEnemy>(this->StaticClass(), FTransform(FRotator::ZeroRotator, RespawnLocation, FVector(1, 1, 1)), SpawnParams);
    if (SpawnedEnemy) {
        AAIController* AIController = GetWorld()->SpawnActor<AAIController>();
        if (AIController) {
            AIController->Possess(SpawnedEnemy);
			print(SpawnedEnemy->GetController()->GetName());
			AIController->MoveToLocation(MoveToTargetLocation);
		}
        this->Destroy();
    }
}

void ANPCEnemy::SetNPCsCollisionContainer(){
	FCollisionResponseContainer CollisionResponseContainer;
	CollisionResponseContainer.SetResponse(ECollisionChannel::ECC_Visibility, ECollisionResponse::ECR_Ignore);
	CollisionResponseContainer.SetResponse(ECollisionChannel::ECC_Camera, ECollisionResponse::ECR_Block);
	CollisionResponseContainer.SetResponse(ECollisionChannel::ECC_WorldStatic, ECollisionResponse::ECR_Block);
	CollisionResponseContainer.SetResponse(ECollisionChannel::ECC_WorldDynamic, ECollisionResponse::ECR_Block);
	CollisionResponseContainer.SetResponse(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Ignore);
	CollisionResponseContainer.SetResponse(ECollisionChannel::ECC_PhysicsBody, ECollisionResponse::ECR_Block);
	CollisionResponseContainer.SetResponse(ECollisionChannel::ECC_Vehicle, ECollisionResponse::ECR_Ignore);
	CollisionResponseContainer.SetResponse(ECollisionChannel::ECC_Destructible, ECollisionResponse::ECR_Block);
	
	GetMesh()->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	GetMesh()->SetCollisionResponseToChannels(CollisionResponseContainer);
}

void ANPCEnemy::BodyHit(UPrimitiveComponent *HitComp, AActor *OtherActor, UPrimitiveComponent *OtherComp, FVector NormalImpulse, const FHitResult &Hit){
	if(ABaseBullet *HitBullet = Cast<ABaseBullet>(OtherActor)){
		if(GetNPCsIsDead()){
			GetMesh()->SetAllBodiesSimulatePhysics(true);
			GetMesh()->AddImpulse(Hit.ImpactNormal * 5000.0f, Hit.BoneName);
		}
	}
}