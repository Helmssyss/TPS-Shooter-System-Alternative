#include "PistolMagazineItem.h"

#include "Components/StaticMeshComponent.h"

#include "UObject/ConstructorHelpers.h"

APistolMagazineItem::APistolMagazineItem(){
	MagazineMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("PISTOL_MAGAZINE"));
	MagazineMesh->SetupAttachment(GetRootComponent());

	CollectibleMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("PISTOL_CollectibleMesh"));
	CollectibleMesh->SetupAttachment(MagazineMesh);

	FCollisionResponseContainer CollisionResponseContainer;
	CollisionResponseContainer.SetResponse(ECollisionChannel::ECC_Visibility, ECollisionResponse::ECR_Block);
	CollisionResponseContainer.SetResponse(ECollisionChannel::ECC_Camera, ECollisionResponse::ECR_Ignore);
	CollisionResponseContainer.SetResponse(ECollisionChannel::ECC_WorldStatic, ECollisionResponse::ECR_Block);
	CollisionResponseContainer.SetResponse(ECollisionChannel::ECC_WorldDynamic, ECollisionResponse::ECR_Block);
	CollisionResponseContainer.SetResponse(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Ignore);
	CollisionResponseContainer.SetResponse(ECollisionChannel::ECC_PhysicsBody, ECollisionResponse::ECR_Ignore);
	CollisionResponseContainer.SetResponse(ECollisionChannel::ECC_Vehicle, ECollisionResponse::ECR_Ignore);
	CollisionResponseContainer.SetResponse(ECollisionChannel::ECC_Destructible, ECollisionResponse::ECR_Block);
	
	MagazineMesh->SetCollisionResponseToChannels(CollisionResponseContainer);

	static ConstructorHelpers::FObjectFinder<UStaticMesh> GetMagazineMesh(TEXT("/Game/_Weapons/Meshes/SMG11/SM_SMG11_Mag_Empty_X"));
	static ConstructorHelpers::FObjectFinder<UStaticMesh> GetCollectibleMesh(TEXT("/Game/_Weapons/Pickups/Pistol_Pickup"));
	
	if(GetMagazineMesh.Succeeded()){
		MagazineMesh->SetStaticMesh(GetMagazineMesh.Object);
		CollectibleMesh->SetStaticMesh(GetCollectibleMesh.Object);
		ReloadAnimation = LoadObject<UAnimationAsset>(nullptr, TEXT("/Game/_Weapons/_WeaponAnimations/PistolReloadSlideAnimation"));
	}
	PrimaryActorTick.bCanEverTick = false;
}

void APistolMagazineItem::BeginPlay(){
	Super::BeginPlay();
	MagazineMesh->SetHiddenInGame(true);
}