#include "WeaponAK47.h"

#include "Components/Image.h"
#include "Components/CapsuleComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/StaticMeshComponent.h"

#include "UObject/ConstructorHelpers.h"
#include "Sound/SoundBase.h"
#include "Particles/ParticleSystem.h"
#include "Animation/AnimationAsset.h"

#define print(x) GEngine->AddOnScreenDebugMessage(-1,2,FColor::Cyan,FString::Printf(TEXT("%s"),*FString(x)))

AWeaponAK47::AWeaponAK47(){
	WeaponMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("WeaponMesh_AK47"));
	WeaponMesh->SetupAttachment(GetRootComponent());
	WeaponMesh->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	WeaponMesh->SetSimulatePhysics(false);

	WeaponHideMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("WeaponHideMesh_AK47"));
	WeaponHideMesh->SetupAttachment(WeaponMesh);

	CapsuleComponent = CreateDefaultSubobject<UCapsuleComponent>(TEXT("CapsuleComponent_AK47"));
	CapsuleComponent->SetupAttachment(WeaponMesh);
	CapsuleComponent->SetCapsuleRadius(15.773f);
	CapsuleComponent->SetRelativeRotation(FRotator(0,0,90.0f));
	CapsuleComponent->SetRelativeLocation(FVector(-0.000221,17.896f,3.775f));
	CapsuleComponent->SetCollisionProfileName(FName("OverlapAllDynamic"));

	PrimaryActorTick.bCanEverTick = false;
	
	SetCollisionContainer();
	static ConstructorHelpers::FObjectFinder<USkeletalMesh> MeshAsset(TEXT("/Game/_Weapons/Meshes/Ka47/SK_KA47"));
	static ConstructorHelpers::FObjectFinder<UTexture2D> WeaponTexture(TEXT("/Game/_Texture/AK47"));
	static ConstructorHelpers::FObjectFinder<UParticleSystem> MuzzleFlashParticle(TEXT("/Game/_FX/P_AssaultRifle_MuzzleFlash"));
	static ConstructorHelpers::FObjectFinder<UTexture2D> WeaponCrosshair(TEXT("/Game/_Texture/RifleCrosshair"));
	static ConstructorHelpers::FObjectFinder<USoundBase> GetWeaponFireSound(TEXT("/Game/_Sounds/Rifle/Cues/RifleA_FireEnd_Cue"));
	static ConstructorHelpers::FObjectFinder<USoundBase> GetWeaponEmptyFireSound(TEXT("/Game/_Sounds/Rifle/Wavs/Empty_gun_shot"));
	static ConstructorHelpers::FObjectFinder<USoundAttenuation> GetWeaponFireAttenuation(TEXT("/Game/_Sounds/Attenuation/WeaponShot_att"));
	static ConstructorHelpers::FObjectFinder<UTexture2D> GetFireModeAuto(TEXT("/Game/_Texture/auto"));
	static ConstructorHelpers::FObjectFinder<UTexture2D> GetFireModeSingle(TEXT("/Game/_Texture/Single"));
	static ConstructorHelpers::FObjectFinder<UTexture2D> GetFireModeSemiAuto(TEXT("/Game/_Texture/semi-auto"));
	// static ConstructorHelpers::FClassFinder<ABaseMagazine> GetMagazine(TEXT("/Game/_Blueprints/BP_AK47MagazineItem"));

	if (MeshAsset.Succeeded() && WeaponTexture.Succeeded() && MuzzleFlashParticle.Succeeded() && GetWeaponFireSound.Succeeded()){
        WeaponMesh->SetSkeletalMesh(MeshAsset.Object);
		WeaponHideMesh->SetSkeletalMesh(MeshAsset.Object);
		ak47Texture = WeaponTexture.Object;
		MuzzleParticle = MuzzleFlashParticle.Object;
		rifleCrosshair = WeaponCrosshair.Object;
		ak47FireSound = GetWeaponFireSound.Object;
		ak47SoundAttenuation = GetWeaponFireAttenuation.Object;
		ak47EmptySound = GetWeaponEmptyFireSound.Object;
		ak47FireModeAuto = GetFireModeAuto.Object;
		ak47FireModeSingle = GetFireModeSingle.Object;
		ak47FireModeSemiAuto = GetFireModeSemiAuto.Object;
		// MagazineClass = GetMagazine.Class;
	}

    maxMermi = 20;
    sarjordekiMermi = 30;
    atilanMermi = 30;
}

void AWeaponAK47::BeginPlay(){
	Super::BeginPlay();
	ak47FireAnimation = LoadObject<UAnimationAsset>(nullptr, TEXT("/Game/_Weapons/_WeaponAnimations/AK47FireAnimation"));
	WeaponHideMesh->SetHiddenInGame(true);
	WeaponHideMesh->HideBoneByName("b_gun_mag",EPhysBodyOp::PBO_None);
}

void AWeaponAK47::Tick(float DeltaTime){
	Super::Tick(DeltaTime);
}

void AWeaponAK47::GunReload(){
    if (atilanMermi < sarjordekiMermi){
		int16 eksikMermi = sarjordekiMermi - atilanMermi;
        int16 eklenecekMermi = FMath::Min(eksikMermi, maxMermi);

        atilanMermi += eklenecekMermi;
        maxMermi -= eklenecekMermi;
        kalanMermi = atilanMermi;
    }
}

void AWeaponAK47::SetCollisionContainer(){
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

void AWeaponAK47::SetNoCollision(){
	WeaponMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	CapsuleComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

UTexture2D *AWeaponAK47::GetWeaponFireModeTexture(uint8 currentMode){
	if(currentMode == 0)
		return ak47FireModeAuto;
	else if(currentMode == 1)
		return ak47FireModeSingle;
	else if(currentMode == 2)
		return ak47FireModeSemiAuto;
    return nullptr;
}

void AWeaponAK47::CurrentWeaponEjectBullet(){
	FActorSpawnParameters params;
	params.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	const FTransform makeTransform = FTransform(GetWeaponMesh()->GetSocketRotation("ejectBullet"), GetWeaponMesh()->GetSocketLocation("ejectBullet"),FVector(1,1,1));
	ABaseBullet *EjectSpawnBullet = GetWorld()->SpawnActor<ABaseBullet>(GetWeaponEjectBulletClass(), makeTransform, params);
	if(EjectSpawnBullet){
		EjectSpawnBullet->GetBulletMesh()->GetOwner()->SetLifeSpan(3);
	}
}