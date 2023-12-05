#include "RifleBullet.h"
#include "../Soldier/Soldier.h"
#include "../Widgets/WeaponStatusWidget.h"
#include "../Widgets/BulletHitWidget.h"
#include "../NPCs/BaseNPCsInterface.h"

#include "Components/StaticMeshComponent.h"
#include "Components/SphereComponent.h"
#include "Components/AudioComponent.h"
#include "Components/Image.h"

#include "PhysicalMaterials/PhysicalMaterial.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Particles/ParticleSystemComponent.h"
#include "Particles/ParticleSystem.h"
#include "UObject/ConstructorHelpers.h"
#include "Kismet/GameplayStatics.h"
#include "TimerManager.h"

#define print(x) GEngine->AddOnScreenDebugMessage(-1,2,FColor::Cyan,FString::Printf(TEXT("%s"),*FString(x)))

ARifleBullet::ARifleBullet(){
    ammoCapsule = CreateDefaultSubobject<USphereComponent>(TEXT("RifleBulletCapsule"));
    ammoCapsule->SetupAttachment(GetRootComponent());
    ammoCapsule->SetSphereRadius(4.0f);
    ammoCapsule->SetCollisionProfileName(FName("BlockAllDynamic"));
    ammoCapsule->SetGenerateOverlapEvents(true);
    ammoCapsule->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
    ammoCapsule->SetHiddenInGame(false);

    ammoMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("RifleBullet"));
    ammoMesh->SetupAttachment(ammoCapsule);
    ammoMesh->SetCollisionProfileName(FName("BlockAllDynamic"));
    ammoMesh->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
    ammoMesh->SetRelativeRotation(FRotator(0.0f, -90.0f, 0.0f));

    BulletTrail = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("RifleBulletTrail"));
    BulletTrail->SetupAttachment(ammoMesh);
    BulletTrail->SetRelativeRotation(FRotator(0.0f, -90.0f, 0.0f));

    BulletAudioCurve = CreateDefaultSubobject<UAudioComponent>(TEXT("RifleBulletAudiuCurve"));
    BulletAudioCurve->SetupAttachment(ammoMesh);

    projectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("RifleBulletProjectileMovement"));
    projectileMovement->SetUpdatedComponent(GetRootComponent());
    projectileMovement->InitialSpeed = 10500;
    projectileMovement->MaxSpeed = 10500;
    projectileMovement->bShouldBounce = true;
    projectileMovement->ProjectileGravityScale = 0.2;

    static ConstructorHelpers::FObjectFinder<UStaticMesh> BulletMesh(TEXT("/Game/_Weapons/Meshes/Ammunition/SM_Shell_762x39"));
    static ConstructorHelpers::FObjectFinder<UParticleSystem> GetBulletTrail(TEXT("/Game/_FX/P_AssaultRifle_Tracer_01"));
    static ConstructorHelpers::FObjectFinder<USoundBase> GetBodyHitSound(TEXT("/Game/_Sounds/Rifle/Cues/Rifle_ImpactBody_Cue"));
    static ConstructorHelpers::FObjectFinder<UParticleSystem> GetBulletImpactParticle(TEXT("/Game/_FX/Particles/Hit/P_Concrete"));
    static ConstructorHelpers::FObjectFinder<UParticleSystem> GetBulletBodyImpactParticle(TEXT("/Game/_FX/Particles/Blood/P_Blood_Splat_Cone"));
    static ConstructorHelpers::FObjectFinder<UParticleSystem> GetBulletDefaultImpactParticle(TEXT("/Game/_FX/Particles/Hit/P_Brick"));
    static ConstructorHelpers::FObjectFinder<USoundBase> GetBulletImpactEmitterSound(TEXT("/Game/_Sounds/Rifle/Cues/Rifle_ImpactSurface_Cue"));
    static ConstructorHelpers::FObjectFinder<USoundAttenuation> GetBulletSoundAttenuation(TEXT("/Game/_Sounds/Attenuation/ProjectileImpact_att"));
    const FString GetBulletDecalMaterial(TEXT("/Game/_Materials/M_BulletHoleDecal"));
    const FString GetBulletBloodDecalMaterial(TEXT("/Game/_Materials/M_BloodHoleDecal"));
    // ParticleSystem'/Game/_FX/Particles/Hit/P_Asphalt.P_Asphalt'
    // static ConstructorHelpers::FObjectFinder<USoundAttenuation> GetBulletAK47AttenuationSound(TEXT("/Game/_Sounds/Attenuation/WeaponShot_att"));
    // static ConstructorHelpers::FObjectFinder<USoundBase> GetBulletAK47WhipSound(TEXT("/Game/_Sounds/Rifle/Wavs/Rifle_Whip01"));

    if (BulletMesh.Succeeded() && GetBulletTrail.Succeeded()){
        ammoMesh->SetStaticMesh(BulletMesh.Object);
        BulletTrail->SetTemplate(GetBulletTrail.Object);
        BodyHitSound = GetBodyHitSound.Object;
        BulletImpactEmitterTemplate = GetBulletImpactParticle.Object;
        BulletBodyImpactEmitterTemplate = GetBulletBodyImpactParticle.Object;
        BulletDefaultImpactEmitterTemplate = GetBulletDefaultImpactParticle.Object;
        BulletImpactEmitterSound = GetBulletImpactEmitterSound.Object;
        BulletSoundAttenuation = GetBulletSoundAttenuation.Object;
        BulletHoleDecalMaterial = Cast<UMaterialInterface>(StaticLoadObject(UMaterialInterface::StaticClass(), nullptr, *GetBulletDecalMaterial));
        BulletBloodDecalMaterial = Cast<UMaterialInterface>(StaticLoadObject(UMaterialInterface::StaticClass(), nullptr, *GetBulletBloodDecalMaterial));
        // BulletAudioCurve->AttenuationSettings = GetBulletAK47AttenuationSound.Object;
        // BulletAudioCurve->SetSound(GetBulletAK47WhipSound.Object);
    }
}

void ARifleBullet::BeginPlay(){
    Super::BeginPlay();
    ammoCapsule->OnComponentHit.AddDynamic(this, &ARifleBullet::BulletHit);
    BulletWidget = CreateWidget<UBulletHitWidget>(this->GetWorld()->GetFirstPlayerController(), TBulletWidget);
}

void ARifleBullet::EndPlay(EEndPlayReason::Type EndPlayReason){
    Super::EndPlay(EndPlayReason);
    BulletWidget->RemoveFromParent();
}

void ARifleBullet::BulletHit(UPrimitiveComponent *HitComponent, AActor *OtherActor, UPrimitiveComponent *OtherComp, FVector NormalImpulse, const FHitResult &Hit){
    BulletImpactParticle(OtherComp, Hit);
    if(ASoldier *soldier = Cast<ASoldier>(OtherActor)){
        print("Karaktere Çarptı");
        DestroyBullet();

    }else if(IBaseNPCsInterface *NPC = Cast<IBaseNPCsInterface>(OtherActor)){
        print("NPC'ye Carpti");
        if(NPC->GetNPCsIsEnemy()){
            print("DUSMAN");
            NPC->SetNPCHealth(28);
            if(NPC->GetNPCHealth() <= 28){
                NPC->DeathEnemy();
                NPC->SetNPCsDeadState(true);
            }
            GEngine->AddOnScreenDebugMessage(-1, 2, FColor::Purple, FString::Printf(TEXT("DUSMAN CANI : %i"), NPC->GetNPCHealth()));
        }
        BulletHitAndHide();
        UGameplayStatics::PlaySound2D(GetWorld(), BodyHitSound);

    }else{
        DestroyBullet();
    }
}

void ARifleBullet::BulletHitAndHide(){
    BulletWidget->AddToViewport();
    BulletWidget->SetVisibility(ESlateVisibility::Hidden);
    SetActorHiddenInGame(true);
    ammoMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
    ammoCapsule->SetCollisionEnabled(ECollisionEnabled::NoCollision);
    BulletWidget->SetVisibility(ESlateVisibility::Visible);
    BulletWidget->PlayAnimation(BulletWidget->hitOpacity);
    GetWorld()->GetTimerManager().SetTimer(BulletTimerHandle, this, &ARifleBullet::DestroyBullet, 0.3, false);
}

void ARifleBullet::BulletImpactParticle(UPrimitiveComponent *OtherComp, const FHitResult &Hit){
    const FVector ImpactDirection = Hit.ImpactPoint - GetActorLocation();
    FRotator NewRotation = ImpactDirection.Rotation();
    NewRotation.Pitch += 90.0f;
    FTransform SpawnTransform(NewRotation, Hit.ImpactPoint, FVector(0.5f));

    if(UMaterialInterface* MaterialInterface = OtherComp->GetMaterial(0)){
		const UPhysicalMaterial* GetPhysicalMaterial = MaterialInterface->GetPhysicalMaterial();
        switch (GetPhysicalMaterial->SurfaceType){
            case EPhysicalSurface::SurfaceType1:
                UGameplayStatics::SpawnSoundAtLocation(GetWorld(), BulletImpactEmitterSound, Hit.ImpactPoint, FRotator::ZeroRotator, 1, 1, 0, BulletSoundAttenuation);
                UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), BulletImpactEmitterTemplate, SpawnTransform);
                UGameplayStatics::SpawnDecalAttached(BulletHoleDecalMaterial, FVector(10, 10, 10), OtherComp, NAME_None, Hit.Location, FRotationMatrix::MakeFromX(Hit.Normal).Rotator(), EAttachLocation::KeepWorldPosition, 10);
                break;

            case EPhysicalSurface::SurfaceType2:
                UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), BulletBodyImpactEmitterTemplate, SpawnTransform);
                UGameplayStatics::SpawnDecalAttached(BulletBloodDecalMaterial, FVector(10, 10, 10), OtherComp, NAME_None, Hit.Location, FRotationMatrix::MakeFromX(Hit.Normal).Rotator(), EAttachLocation::KeepWorldPosition, 10);
                break;
            
            case EPhysicalSurface::SurfaceType3:
                UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), BulletDefaultImpactEmitterTemplate, SpawnTransform);
                UGameplayStatics::SpawnDecalAttached(BulletHoleDecalMaterial, FVector(10, 10, 10), OtherComp, NAME_None, Hit.Location, FRotationMatrix::MakeFromX(Hit.Normal).Rotator(), EAttachLocation::KeepWorldPosition, 10);
                break;
        }
    }else{
        UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), BulletDefaultImpactEmitterTemplate, SpawnTransform);
        UGameplayStatics::SpawnSoundAtLocation(GetWorld(), BulletImpactEmitterSound, Hit.ImpactPoint, FRotator::ZeroRotator, 1, 1, 0, BulletSoundAttenuation);
        UGameplayStatics::SpawnDecalAttached(BulletHoleDecalMaterial, FVector(10, 10, 10), OtherComp, NAME_None, Hit.Location, FRotationMatrix::MakeFromX(Hit.Normal).Rotator(), EAttachLocation::KeepWorldPosition, 10);
    }
}