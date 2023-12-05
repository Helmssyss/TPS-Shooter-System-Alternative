#include "WeaponPistol.h"
#include "Particles/ParticleSystem.h"

#include "Components/Image.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/CapsuleComponent.h"

#include "UObject/ConstructorHelpers.h"

AWeaponPistol::AWeaponPistol(){
    WeaponMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("WeaponMesh_Deagle"));
	WeaponMesh->SetupAttachment(GetRootComponent());
	WeaponMesh->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	WeaponMesh->SetSimulatePhysics(false);

	WeaponHideMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("WeaponHideMesh_Deagle"));
	WeaponHideMesh->SetupAttachment(WeaponMesh);

	CapsuleComponent = CreateDefaultSubobject<UCapsuleComponent>(TEXT("CapsuleComponent_Deagle"));
	CapsuleComponent->SetupAttachment(WeaponMesh);
	CapsuleComponent->SetCapsuleRadius(7.605f);
	CapsuleComponent->SetCapsuleHalfHeight(17.240189);
	CapsuleComponent->SetRelativeRotation(FRotator(0,0,90.0f));
	CapsuleComponent->SetRelativeLocation(FVector(0.0f,8.590683,7.281248));
	CapsuleComponent->SetCollisionProfileName(FName("OverlapAllDynamic"));

	PrimaryActorTick.bCanEverTick = false;
	
	SetCollisionContainer();
	static ConstructorHelpers::FObjectFinder<USkeletalMesh> MeshAsset(TEXT("/Game/_Weapons/Advanced/Pistols_A"));
	static ConstructorHelpers::FObjectFinder<UTexture2D> WeaponTexture(TEXT("/Game/_Texture/pistol"));
	static ConstructorHelpers::FObjectFinder<UParticleSystem> MuzzleFlashParticle(TEXT("/Game/_FX/P_Pistol_MuzzleFlash_01"));
	static ConstructorHelpers::FObjectFinder<UTexture2D> WeaponCrosshair(TEXT("/Game/_Texture/RifleCrosshair"));
	static ConstructorHelpers::FObjectFinder<USoundBase> GetWeaponFireSound(TEXT("/Game/_Sounds/Pistol/Cues/PistolA_Fire_Cue"));
	static ConstructorHelpers::FObjectFinder<USoundBase> GetWeaponEmptyFireSound(TEXT("/Game/_Sounds/Rifle/Wavs/Empty_gun_shot"));
	static ConstructorHelpers::FObjectFinder<USoundAttenuation> GetWeaponFireAttenuation(TEXT("/Game/_Sounds/Attenuation/WeaponShot_att"));
	static ConstructorHelpers::FObjectFinder<UTexture2D> GetFireModeSingle(TEXT("/Game/_Texture/Single"));
    if (MeshAsset.Succeeded() && WeaponTexture.Succeeded() && WeaponCrosshair.Succeeded() && GetWeaponFireSound.Succeeded()){
		WeaponMesh->SetSkeletalMesh(MeshAsset.Object);
		WeaponHideMesh->SetSkeletalMesh(MeshAsset.Object);
		MuzzleParticle = MuzzleFlashParticle.Object;
		pistolTexture = WeaponTexture.Object;
		pistolCrosshair = WeaponCrosshair.Object;
		pistolFireSound = GetWeaponFireSound.Object;
		pistolSoundAttenuation = GetWeaponFireAttenuation.Object;
		pistolEmptySound = GetWeaponEmptyFireSound.Object;
		pistolFireModeTexture = GetFireModeSingle.Object;
	}

    maxMermi = 30;
    sarjordekiMermi = 15;
    atilanMermi = 15;
}

void AWeaponPistol::BeginPlay(){
	Super::BeginPlay();
	pistolFireAnimation = LoadObject<UAnimationAsset>(nullptr, TEXT("/Game/_Weapons/_WeaponAnimations/PistolFireAnimation"));
	pistolReloadAnimation = LoadObject<UAnimationAsset>(nullptr, TEXT("/Game/_Weapons/_WeaponAnimations/PistolReloadSlideAnimation"));
	WeaponHideMesh->SetHiddenInGame(true);
	WeaponHideMesh->HideBoneByName("Clip_Bone",EPhysBodyOp::PBO_None);
}

void AWeaponPistol::Tick(float DeltaTime){
	Super::Tick(DeltaTime);
}

void AWeaponPistol::GunReload(){
    if (atilanMermi < sarjordekiMermi){
		int16 eksikMermi = sarjordekiMermi - atilanMermi;
        int16 eklenecekMermi = FMath::Min(eksikMermi, maxMermi);

        atilanMermi += eklenecekMermi;
        maxMermi -= eklenecekMermi;
        kalanMermi = atilanMermi;
    }
}

void AWeaponPistol::SetCollisionContainer(){
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

void AWeaponPistol::SetNoCollision(){
	WeaponMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	CapsuleComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

void AWeaponPistol::CurrentWeaponEjectBullet(){
	FActorSpawnParameters params;
	params.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	const FTransform makeTransform = FTransform(GetWeaponMesh()->GetSocketRotation("ejectBullet"), GetWeaponMesh()->GetSocketLocation("ejectBullet"),FVector(1,1,1));
	ABaseBullet *EjectSpawnBullet = GetWorld()->SpawnActor<ABaseBullet>(GetWeaponEjectBulletClass(), makeTransform, params);
	if(EjectSpawnBullet){
		EjectSpawnBullet->GetBulletMesh()->GetOwner()->SetLifeSpan(3);
	}
}