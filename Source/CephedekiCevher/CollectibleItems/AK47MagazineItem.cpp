#include "AK47MagazineItem.h"

#include "Components/StaticMeshComponent.h"

#include "UObject/ConstructorHelpers.h"

AAK47MagazineItem::AAK47MagazineItem(){
	MagazineMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("AK47_MAGAZINE"));
	MagazineMesh->SetupAttachment(GetRootComponent());

	CollectibleMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("AK47_CollectibleMesh"));
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
	static ConstructorHelpers::FObjectFinder<UStaticMesh> GetMagazineMesh(TEXT("/Game/_Weapons/Meshes/Ka47/SM_KA47_Mag_Empty"));
	static ConstructorHelpers::FObjectFinder<UStaticMesh> GetCollectibleMesh(TEXT("/Game/_Weapons/Pickups/AssaultRifle_Pickup"));

	if(GetMagazineMesh.Succeeded()){
		MagazineMesh->SetStaticMesh(GetMagazineMesh.Object);
		CollectibleMesh->SetStaticMesh(GetCollectibleMesh.Object);
	}
	PrimaryActorTick.bCanEverTick = false;
}

void AAK47MagazineItem::BeginPlay(){
	Super::BeginPlay();
	MagazineMesh->SetHiddenInGame(true);
}

void AAK47MagazineItem::Tick(float DeltaTime){
	Super::Tick(DeltaTime);
}