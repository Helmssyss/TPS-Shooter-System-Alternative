#include "WeaponAR4.h"

#include "Particles/ParticleSystem.h"

#include "Components/Image.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/CapsuleComponent.h"

#include "UObject/ConstructorHelpers.h"

AWeaponAR4::AWeaponAR4(){
	WeaponMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("WeaponMesh_AR4"));
	WeaponMesh->SetupAttachment(GetRootComponent());
	WeaponMesh->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	WeaponMesh->SetSimulatePhysics(false);

	WeaponHideMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("WeaponHideMesh_AR4"));
	WeaponHideMesh->SetupAttachment(WeaponMesh);
	
	CapsuleComponent = CreateDefaultSubobject<UCapsuleComponent>(TEXT("CapsuleComponent_AR4"));
	CapsuleComponent->SetupAttachment(WeaponMesh);
	CapsuleComponent->SetCapsuleRadius(15.773f);
	CapsuleComponent->SetRelativeRotation(FRotator(0,0,90.0f));
	CapsuleComponent->SetRelativeLocation(FVector(-0.0f, 17.89f, 5.94f));
	CapsuleComponent->SetCollisionProfileName(FName("OverlapAllDynamic"));

	// WeaponHideMesh->SetRelativeRotation(FRotator(0,0,90.0f));
	// WeaponHideMesh->SetRelativeLocation(WeaponMesh->GetRelativeLocation());

	PrimaryActorTick.bCanEverTick = false;
	
	SetCollisionContainer();
	static ConstructorHelpers::FObjectFinder<USkeletalMesh> MeshAsset(TEXT("/Game/_Weapons/Meshes/AR4/SK_AR4"));
	static ConstructorHelpers::FObjectFinder<UTexture2D> WeaponTexture(TEXT("/Game/_Texture/assault-rifle"));
	static ConstructorHelpers::FObjectFinder<UParticleSystem> MuzzleFlashParticle(TEXT("/Game/_FX/P_AssaultRifle_MuzzleFlash"));
	static ConstructorHelpers::FObjectFinder<UTexture2D> WeaponCrosshair(TEXT("/Game/_Texture/RifleCrosshair"));
	static ConstructorHelpers::FObjectFinder<USoundBase> GetWeaponFireSound(TEXT("/Game/_Sounds/Rifle/Cues/RifleA_Fire_Cue"));
	static ConstructorHelpers::FObjectFinder<USoundBase> GetWeaponEmptyFireSound(TEXT("/Game/_Sounds/Rifle/Wavs/Empty_gun_shot"));
	static ConstructorHelpers::FObjectFinder<USoundAttenuation> GetWeaponFireAttenuation(TEXT("/Game/_Sounds/Attenuation/WeaponShot_att.WeaponShot_att"));
	static ConstructorHelpers::FObjectFinder<UTexture2D> GetFireModeAuto(TEXT("/Game/_Texture/auto"));
	static ConstructorHelpers::FObjectFinder<UTexture2D> GetFireModeSingle(TEXT("/Game/_Texture/Single"));
	static ConstructorHelpers::FObjectFinder<UTexture2D> GetFireModeSemiAuto(TEXT("/Game/_Texture/semi-auto"));
	if (MeshAsset.Succeeded() && WeaponTexture.Succeeded() && WeaponCrosshair.Object){
        WeaponMesh->SetSkeletalMesh(MeshAsset.Object);
		WeaponHideMesh->SetSkeletalMesh(MeshAsset.Object);
		MuzzleParticle = MuzzleFlashParticle.Object;
		ar4Texture = WeaponTexture.Object;
		rifleCrosshair = WeaponCrosshair.Object;
		ar4FireSound = GetWeaponFireSound.Object;
		ar4SoundAttenuation = GetWeaponFireAttenuation.Object;
		ar4EmptySound = GetWeaponEmptyFireSound.Object;
		ar4FireModeAuto = GetFireModeAuto.Object;
		ar4FireModeSingle = GetFireModeSingle.Object;
		ar4FireModeSemiAuto = GetFireModeSemiAuto.Object;
	}

    maxMermi = 20;
    sarjordekiMermi = 25;
    atilanMermi = 25;
}

void AWeaponAR4::BeginPlay(){
    Super::BeginPlay();
	ar4FireAnimation = LoadObject<UAnimationAsset>(nullptr, TEXT("/Game/_Weapons/_WeaponAnimations/AR4FireAnimation"));
	WeaponHideMesh->SetHiddenInGame(true);
	WeaponHideMesh->HideBoneByName("b_gun_mag",EPhysBodyOp::PBO_None);
}

void AWeaponAR4::Tick(float DeltaTime){
    Super::Tick(DeltaTime);
}

void AWeaponAR4::GunReload(){
    if (atilanMermi < sarjordekiMermi){
		int16 eksikMermi = sarjordekiMermi - atilanMermi;
        int16 eklenecekMermi = FMath::Min(eksikMermi, maxMermi);

        atilanMermi += eklenecekMermi;
        maxMermi -= eklenecekMermi;
        kalanMermi = atilanMermi;
    }
}

void AWeaponAR4::SetCollisionContainer(){
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

void AWeaponAR4::SetNoCollision(){
	WeaponMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	CapsuleComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

UTexture2D *AWeaponAR4::GetWeaponFireModeTexture(uint8 currentMode){
	if(currentMode == 0)
		return ar4FireModeAuto;
	else if(currentMode == 1)
		return ar4FireModeSingle;
	else if(currentMode == 2)
		return ar4FireModeSemiAuto;
    return nullptr;
}

void AWeaponAR4::CurrentWeaponEjectBullet(){
	FActorSpawnParameters params;
	params.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	const FTransform makeTransform = FTransform(GetWeaponMesh()->GetSocketRotation("ejectBullet"), GetWeaponMesh()->GetSocketLocation("ejectBullet"),FVector(1,1,1));
	ABaseBullet *EjectSpawnBullet = GetWorld()->SpawnActor<ABaseBullet>(GetWeaponEjectBulletClass(), makeTransform, params);
	if(EjectSpawnBullet){
		EjectSpawnBullet->GetBulletMesh()->GetOwner()->SetLifeSpan(3);
	}
}