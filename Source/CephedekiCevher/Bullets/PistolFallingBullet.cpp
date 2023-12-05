#include "PistolFallingBullet.h"

#include "Components/StaticMeshComponent.h"
#include "Components/AudioComponent.h"

#include "GameFramework/ProjectileMovementComponent.h"
#include "GameFramework/RotatingMovementComponent.h"

#include "UObject/ConstructorHelpers.h"
#include "Kismet/GameplayStatics.h"

#define print(x) GEngine->AddOnScreenDebugMessage(-1,2,FColor::Cyan,FString::Printf(TEXT("%s"),*FString(x)))

APistolFallingBullet::APistolFallingBullet(){
    ammoMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("PistolBulletFalling"));
    ammoMesh->SetupAttachment(GetRootComponent());
    ammoMesh->SetCollisionProfileName(FName("BlockAllDynamic"));
    ammoMesh->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);

    // BulletAudioCurve = CreateDefaultSubobject<UAudioComponent>(TEXT("RocketLauncherBulletAudio"));
  	// BulletAudioCurve->SetupAttachment(ammoMesh);

    BulletRotatingMovementComponent = CreateDefaultSubobject<URotatingMovementComponent>(TEXT("PistolFallingBullet"));
    BulletRotatingMovementComponent->SetUpdatedComponent(GetRootComponent());
    BulletRotatingMovementComponent->RotationRate = FRotator(1000, 0, -1000);

    projectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("PistolFallingBulletProjectileMovement"));
    projectileMovement->SetUpdatedComponent(GetRootComponent());
    projectileMovement->InitialSpeed = 150;
    projectileMovement->MaxSpeed = 150;
    projectileMovement->bShouldBounce = true;
    projectileMovement->ProjectileGravityScale = 1.0f;
    projectileMovement->bRotationFollowsVelocity = false;
    projectileMovement->Velocity = FVector(1, 0, 1);

    FCollisionResponseContainer CollisionResponseContainer;
	CollisionResponseContainer.SetResponse(ECollisionChannel::ECC_Visibility, ECollisionResponse::ECR_Block);
	CollisionResponseContainer.SetResponse(ECollisionChannel::ECC_Camera, ECollisionResponse::ECR_Ignore);
	CollisionResponseContainer.SetResponse(ECollisionChannel::ECC_WorldStatic, ECollisionResponse::ECR_Block);
	CollisionResponseContainer.SetResponse(ECollisionChannel::ECC_WorldDynamic, ECollisionResponse::ECR_Block);
	CollisionResponseContainer.SetResponse(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Ignore);
	CollisionResponseContainer.SetResponse(ECollisionChannel::ECC_PhysicsBody, ECollisionResponse::ECR_Ignore);
	CollisionResponseContainer.SetResponse(ECollisionChannel::ECC_Vehicle, ECollisionResponse::ECR_Ignore);
	CollisionResponseContainer.SetResponse(ECollisionChannel::ECC_Destructible, ECollisionResponse::ECR_Block);
	
	ammoMesh->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	ammoMesh->SetCollisionResponseToChannels(CollisionResponseContainer);

    static ConstructorHelpers::FObjectFinder<UStaticMesh> GetEmptyBullet(TEXT("/Game/_Weapons/Meshes/Ammunition/SM_Shell_9mm_Empty"));
    static ConstructorHelpers::FObjectFinder<USoundBase> GetEmptyBulletSound(TEXT("/Game/_Sounds/BulletFallingCue"));
    ammoMesh->SetStaticMesh(GetEmptyBullet.Object);
    FallingSound = GetEmptyBulletSound.Object;
    // BulletAudioCurve->SetSound(GetEmptyBulletSound.Object);
}

void APistolFallingBullet::BeginPlay(){
    Super::BeginPlay();
    ammoMesh->OnComponentHit.AddDynamic(this, &APistolFallingBullet::BulletHit);
}


void APistolFallingBullet::BulletHit(UPrimitiveComponent *HitComponent, AActor *OtherActor, UPrimitiveComponent *OtherComp, FVector NormalImpulse, const FHitResult &Hit){
    print("YereDegdi");
    UGameplayStatics::PlaySoundAtLocation(GetWorld(), FallingSound, Hit.ImpactPoint);
    Destroy();
}