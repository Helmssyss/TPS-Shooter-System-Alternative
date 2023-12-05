#include "WeaponRocketLauncher.h"
#include "Particles/ParticleSystem.h"

#include "Components/Image.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/CapsuleComponent.h"

#include "UObject/ConstructorHelpers.h"

AWeaponRocketLauncher::AWeaponRocketLauncher(){
    WeaponMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("WeaponMesh_RocketLaunch"));
	WeaponMesh->SetupAttachment(GetRootComponent());
	WeaponMesh->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	WeaponMesh->SetSimulatePhysics(false);

	WeaponHideMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("WeaponHideMesh_RocketLaunch"));
	WeaponHideMesh->SetupAttachment(WeaponMesh);

	CapsuleComponent = CreateDefaultSubobject<UCapsuleComponent>(TEXT("CapsuleComponent_RocketLaunch"));
	CapsuleComponent->SetupAttachment(WeaponMesh);

	CapsuleComponent->SetCapsuleRadius(15.773f);
	CapsuleComponent->SetCapsuleHalfHeight(59.56f);
	CapsuleComponent->SetRelativeRotation(FRotator(0,0,90.0f));
	CapsuleComponent->SetRelativeLocation(FVector(0.0f,-3.332223,9.188993));
	CapsuleComponent->SetCollisionProfileName(FName("OverlapAllDynamic"));

	PrimaryActorTick.bCanEverTick = false;
	
	SetCollisionContainer();
	static ConstructorHelpers::FObjectFinder<USkeletalMesh> MeshAsset(TEXT("/Game/_Weapons/Advanced/Rocket_Launcher_A"));
	static ConstructorHelpers::FObjectFinder<UTexture2D> WeaponTexture(TEXT("/Game/_Texture/RocketLauncher"));
	static ConstructorHelpers::FObjectFinder<UTexture2D> WeaponCrosshair(TEXT("/Game/_Texture/RocketLauncherCrosshair"));
	static ConstructorHelpers::FObjectFinder<UParticleSystem> MuzzleFlashParticle(TEXT("/Game/_FX/P_RocketLauncher_MuzzleFlash_Front_01"));
	static ConstructorHelpers::FObjectFinder<USoundBase> GetWeaponFireSound(TEXT("/Game/_Sounds/RocketLauncher/Cues/RocketLauncherA_Fire_Cue"));
	static ConstructorHelpers::FObjectFinder<USoundAttenuation> GetWeaponFireAttenuation(TEXT("/Game/_Sounds/Attenuation/WeaponShot_att"));
	static ConstructorHelpers::FObjectFinder<UParticleSystem> GetMuzzleFlashRear(TEXT("/Game/_FX/P_RocketLauncher_MuzzleFlash_Rear_01"));

	if (MeshAsset.Succeeded() && WeaponTexture.Succeeded() && WeaponCrosshair.Succeeded() && MuzzleFlashParticle.Succeeded()){
        WeaponMesh->SetSkeletalMesh(MeshAsset.Object);
		WeaponHideMesh->SetSkeletalMesh(MeshAsset.Object);
		MuzzleParticle = MuzzleFlashParticle.Object;
		rocketLauncherTexture = WeaponTexture.Object;
		rocketLauncherCrosshair = WeaponCrosshair.Object;
		rocketLauncherFireSound = GetWeaponFireSound.Object;
		rocketLauncherSoundAttenuation = GetWeaponFireAttenuation.Object;
		MuzzleFlashRear = GetMuzzleFlashRear.Object;
	}
    maxMermi = 3;
    sarjordekiMermi = 1;
    atilanMermi = 1;
}

void AWeaponRocketLauncher::BeginPlay(){
    Super::BeginPlay();
	WeaponHideMesh->SetHiddenInGame(true);
	WeaponHideMesh->HideBoneByName("Clip_Bone",EPhysBodyOp::PBO_None);
}

void AWeaponRocketLauncher::Tick(float DeltaTime){
    Super::Tick(DeltaTime);
}

void AWeaponRocketLauncher::GunReload(){
    if (atilanMermi < sarjordekiMermi){
		int16 eksikMermi = sarjordekiMermi - atilanMermi;
        int16 eklenecekMermi = FMath::Min<int16>(eksikMermi, maxMermi);

        atilanMermi += eklenecekMermi;
        maxMermi -= eklenecekMermi;
        kalanMermi = atilanMermi;
    }
}

void AWeaponRocketLauncher::SetCollisionContainer(){
	FCollisionResponseContainer CollisionResponseContainer;
	CollisionResponseContainer.SetResponse(ECollisionChannel::ECC_Visibility, ECollisionResponse::ECR_Block);
	CollisionResponseContainer.SetResponse(ECollisionChannel::ECC_Camera, ECollisionResponse::ECR_Ignore);
	CollisionResponseContainer.SetResponse(ECollisionChannel::ECC_WorldStatic, ECollisionResponse::ECR_Block);
	CollisionResponseContainer.SetResponse(ECollisionChannel::ECC_WorldDynamic, ECollisionResponse::ECR_Block);
	CollisionResponseContainer.SetResponse(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Ignore);
	CollisionResponseContainer.SetResponse(ECollisionChannel::ECC_PhysicsBody, ECollisionResponse::ECR_Ignore);
	CollisionResponseContainer.SetResponse(ECollisionChannel::ECC_Vehicle, ECollisionResponse::ECR_Ignore);
	CollisionResponseContainer.SetResponse(ECollisionChannel::ECC_Destructible, ECollisionResponse::ECR_Block);

	WeaponMesh->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	WeaponMesh->SetCollisionResponseToChannels(CollisionResponseContainer);
	CapsuleComponent->SetCollisionProfileName("OverlapAllDynamic");
}

void AWeaponRocketLauncher::SetNoCollision(){
	WeaponMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	CapsuleComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}