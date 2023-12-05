#include "RocketLauncherBullet.h"
#include "../Soldier/Soldier.h"
#include "../NPCs/BaseNPCsInterface.h"
#include "../Widgets/BulletHitWidget.h"

#include "Components/StaticMeshComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/SphereComponent.h"
#include "Components/AudioComponent.h"

#include "GameFramework/ProjectileMovementComponent.h"
#include "Particles/ParticleSystemComponent.h"
#include "Particles/ParticleSystem.h"
#include "Kismet/GameplayStatics.h"
#include "UObject/ConstructorHelpers.h"
#include "TimerManager.h"
#include "DrawDebugHelpers.h"

#define print(x) GEngine->AddOnScreenDebugMessage(-1,2,FColor::Cyan,FString::Printf(TEXT("%s"),*FString(x)))

ARocketLauncherBullet::ARocketLauncherBullet(){
    ammoCapsule = CreateDefaultSubobject<USphereComponent>(TEXT("RocketLauncherBulletCAPSULE"));
    ammoCapsule->SetupAttachment(GetRootComponent());
    ammoCapsule->SetSphereRadius(4.0f);
    ammoCapsule->SetCollisionProfileName(FName("BlockAllDynamic"));
    ammoCapsule->SetGenerateOverlapEvents(true);
    ammoCapsule->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
    ammoCapsule->SetHiddenInGame(false);

    ammoMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("RocketLauncherBullet"));
    ammoMesh->SetupAttachment(ammoCapsule);
    ammoMesh->SetCollisionProfileName(FName("BlockAllDynamic"));
    ammoMesh->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
    // ammoMesh->SetRelativeRotation(FRotator(0.0f, -90.0f, 0.0f));
	
    BulletAudioCurve = CreateDefaultSubobject<UAudioComponent>(TEXT("RocketLauncherBulletAudio"));
  	BulletAudioCurve->SetupAttachment(ammoMesh);

    BulletTrail = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("RocketLauncherBulletTrail"));
    BulletTrail->SetupAttachment(ammoMesh);
    BulletTrail->SetRelativeLocation(FVector(-12.41f, 0.0f, 0.0f));

    projectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("RocketLauncherBulletProjectileMovement"));
    projectileMovement->SetUpdatedComponent(GetRootComponent());
    projectileMovement->InitialSpeed = 5000;
    projectileMovement->MaxSpeed = 5000;
    projectileMovement->bShouldBounce = true;
    projectileMovement->ProjectileGravityScale = 0.2;

    static ConstructorHelpers::FObjectFinder<UStaticMesh> BulletMesh(TEXT("/Game/_Weapons/Advanced/RocketLauncherA_Ammo"));
    static ConstructorHelpers::FObjectFinder<UParticleSystem> GetBulletTrail(TEXT("/Game/_FX/P_RocketLauncher_Trail_01"));
    static ConstructorHelpers::FObjectFinder<UParticleSystem> GetBulletExplosion(TEXT("/Game/_FX/Particles/Explosion/P_Explosion_Big_B"));
    static ConstructorHelpers::FObjectFinder<USoundBase> GetBulletExplosionSound(TEXT("/Game/_Sounds/RocketLauncher/Cues/RocketLauncher_Explosion_Cue"));
    static ConstructorHelpers::FObjectFinder<USoundAttenuation> GetBulletExplosionAttenuationSound(TEXT("/Game/_Sounds/Attenuation/Explosion_att"));
    static ConstructorHelpers::FObjectFinder<USoundBase> GetBulletAirLoopSound(TEXT("/Game/_Sounds/RocketLauncher/Cues/RocketLauncher_InAirLoop_Cue"));
    static ConstructorHelpers::FObjectFinder<USoundAttenuation> GetBulletAirLoopAttenuationSound(TEXT("/Game/_Sounds/Attenuation/RocketLoop_att"));

    if (BulletMesh.Succeeded() && GetBulletTrail.Succeeded() && GetBulletExplosion.Succeeded()){
        ammoMesh->SetStaticMesh(BulletMesh.Object);
        BulletTrail->SetTemplate(GetBulletTrail.Object);
        BulletHitExplosionParticle = GetBulletExplosion.Object;
        ExplosionSound = GetBulletExplosionSound.Object;
        ExplosionAttenuationSound = GetBulletExplosionAttenuationSound.Object;

        BulletAudioCurve->AttenuationSettings = GetBulletAirLoopAttenuationSound.Object;
        BulletAudioCurve->SetSound(GetBulletAirLoopSound.Object);
    }
}

void ARocketLauncherBullet::BeginPlay(){
    Super::BeginPlay();
    ammoCapsule->OnComponentHit.AddDynamic(this, &ARocketLauncherBullet::BulletHit);
    BulletAudioCurve->OnAudioFinished.AddDynamic(this, &ARocketLauncherBullet::OnAudioFinished);
    BulletWidget = CreateWidget<UBulletHitWidget>(this->GetWorld()->GetFirstPlayerController(), TBulletWidget);
}

void ARocketLauncherBullet::EndPlay(EEndPlayReason::Type EndPlayReason){
    Super::EndPlay(EndPlayReason);
    print("Sa");
    // BulletWidget->RemoveFromParent();
}

void ARocketLauncherBullet::OnAudioFinished(){
    if (BulletAudioCurve){
        BulletAudioCurve->Play();
        float SoundDuration = BulletAudioCurve->Sound ? BulletAudioCurve->Sound->Duration : 0.0f;
        float TimeUntilRestart = FMath::Max(SoundDuration - 1.0f, 0.0f);
        GetWorld()->GetTimerManager().SetTimer(TimerHandle_AudioRestart, this, &ARocketLauncherBullet::PlayAudio, TimeUntilRestart, false);
    }
}

void ARocketLauncherBullet::PlayAudio(){
    if (BulletAudioCurve){
        BulletAudioCurve->Play();
    }
}

void ARocketLauncherBullet::BulletHit(UPrimitiveComponent *HitComponent, AActor *OtherActor, UPrimitiveComponent *OtherComp, FVector NormalImpulse, const FHitResult &Hit){
    
    UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), BulletHitExplosionParticle, FTransform(this->GetActorRotation(), Hit.ImpactPoint, FVector(1.5, 1.5, 1.5)));
    UGameplayStatics::SpawnSoundAtLocation(GetWorld(), ExplosionSound, Hit.ImpactPoint, GetActorRotation(),1,1,0,ExplosionAttenuationSound);
    
    TArray<FHitResult> Hits;
    const float SphereRadius = 400.0f;
    bool isHit = GetWorld()->SweepMultiByChannel(Hits, GetActorLocation(), GetActorLocation(), FQuat::Identity, ECollisionChannel::ECC_Pawn, FCollisionShape::MakeSphere(SphereRadius));
    DrawDebugSphere(GetWorld(), Hit.ImpactPoint, FCollisionShape::MakeSphere(SphereRadius).GetSphereRadius(), 50, FColor::Cyan, false, 3);
    if (isHit){
        for (auto& i : Hits){
            if (UStaticMeshComponent* MeshComp = Cast<UStaticMeshComponent>(i.GetActor()->GetRootComponent())){ // herhangi bir static mesh komponentine sahip aktör
                MeshComp->AddRadialImpulse(GetActorLocation(), SphereRadius, 2000.f, ERadialImpulseFalloff::RIF_Constant, true);

            }
            if(ASoldier *Character = Cast<ASoldier>(i.GetActor())){
                print("Karaktere Çarptı");
                Character->SetSoldierHealth(50);
                if(Character->GetSoldierHealth() == 0 ){
                    GEngine->AddOnScreenDebugMessage(-1, 2, FColor::Red, FString::Printf(TEXT("%i"), Character->GetSoldierHealth()));
                    Character->GetMesh()->SetAllBodiesBelowSimulatePhysics(FName("Hips"),true);
                    Character->GetMesh()->AddRadialImpulse(GetActorLocation(), SphereRadius, 2000.f, ERadialImpulseFalloff::RIF_Linear, true);
                    Character->SoldierDeath();
                }
            }
            if(IBaseNPCsInterface *NPC = Cast<IBaseNPCsInterface>(i.GetActor())){
                print("NPC'ye Carpti");
                NPC->GetNPCMesh()->SetAllBodiesBelowSimulatePhysics(FName("Hips"),true);
                NPC->GetNPCMesh()->AddRadialImpulse(GetActorLocation(), SphereRadius, 2000.f, ERadialImpulseFalloff::RIF_Linear, true);
                if(NPC->GetNPCsIsEnemy()){
                    NPC->DeathEnemy();
                    NPC->SetNPCsDeadState(true);
                }
                BulletHitAndHide();
            }
        }
    }
    DestroyBullet();
}

void ARocketLauncherBullet::BulletHitAndHide(){
    if(!BulletWidget->IsInViewport()){
        BulletWidget->AddToViewport();
        BulletWidget->SetVisibility(ESlateVisibility::Hidden);
        SetActorHiddenInGame(true);
        ammoMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
        ammoCapsule->SetCollisionEnabled(ECollisionEnabled::NoCollision);
        BulletWidget->SetVisibility(ESlateVisibility::Visible);
        BulletWidget->PlayAnimation(BulletWidget->hitOpacity);
    }
    GetWorld()->GetTimerManager().SetTimer(BulletTimerHandle, this, &ARocketLauncherBullet::DestroyBullet, 0.3, false);
}