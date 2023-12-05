#include "Soldier.h"
#include "../Weapons/BaseWeaponInterface.h"
#include "../Bullets/BaseBullet.h"
#include "../Widgets/WeaponInteractWidget.h"
#include "../Widgets/WeaponStatusWidget.h"
#include "../CephedekiCevherGameMode.h"

#include "Camera/CameraComponent.h"
#include "Camera/CameraShakeBase.h"

#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "Components/ChildActorComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/SphereComponent.h"
#include "Components/SceneComponent.h"
#include "Components/TimelineComponent.h"
#include "Components/Image.h"

#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Controller.h"
#include "GameFramework/PlayerController.h"
#include "GameFramework/SpringArmComponent.h"

#include "Animation/AnimInstance.h"
#include "HeadMountedDisplayFunctionLibrary.h"
#include "DrawDebugHelpers.h"
#include "TimerManager.h"
#include "Blueprint/UserWidget.h"
#include "Kismet/GameplayStatics.h"
#include "UObject/ConstructorHelpers.h"
#include "Curves/CurveFloat.h"

#define print(x) GEngine->AddOnScreenDebugMessage(-1,2,FColor::Cyan,FString::Printf(TEXT("%s"),*FString(x)))

ASoldier::ASoldier(){
	GetCapsuleComponent()->InitCapsuleSize(12.005492, 74.917404);
	GetMesh()->SetRelativeLocation(FVector(-0.289703, -2.602029, -78.006790));
	GetMesh()->SetRelativeRotation(FRotator(0.0, -90.0, 0.0));

	BaseTurnRate = 45.f;
	BaseLookUpRate = 45.f;

	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->bUseControllerDesiredRotation = false;
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 540.0f, 0.0f);
	GetCharacterMovement()->JumpZVelocity = 400.f;
	GetCharacterMovement()->AirControl = 0.2f;
	GetCharacterMovement()->MaxWalkSpeed = 166.0f;

	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(GetMesh());
	CameraBoom->SetRelativeLocation(FVector(0.0,19.707230,166.882416));
	CameraBoom->SocketOffset = FVector(0.0,22.466288,0.0);
	CameraBoom->TargetArmLength = 300.0f;
	CameraBoom->bUsePawnControlRotation = true;

	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, "Head");
	FollowCamera->SetRelativeRotation(FRotator::ZeroRotator);
	FollowCamera->bUsePawnControlRotation = false;

	SelectItemCapsule = CreateDefaultSubobject<UCapsuleComponent>(TEXT("SelectItemCapsule"));
	SelectItemCapsule->SetupAttachment(FollowCamera);
	SelectItemCapsule->SetRelativeRotation(FRotator(-89, 0.0, 0.0));
	SelectItemCapsule->SetCapsuleRadius(0);
	SelectItemCapsule->SetCapsuleHalfHeight(258.196655);
	SelectItemCapsule->SetRelativeLocation(FVector(242.153305,0.002839,-0.238446));

	WeaponsStatusMap.Add("ChildRifleActor", false);
	WeaponsStatusMap.Add("ChildPistolActor", false);
	WeaponsStatusMap.Add("ChildRocketLaunchActor", false);

	PrimaryActorTick.bCanEverTick = true;
	static ConstructorHelpers::FClassFinder<UUserWidget> WeaponInteractWidgetClass(TEXT("/Game/_Blueprints/_BP_Widgets/BP_WeaponInteractWidget"));
	static ConstructorHelpers::FClassFinder<UUserWidget> WeaponStatusWidgetClass(TEXT("/Game/_Blueprints/_BP_Widgets/BP_WeaponStatusWidget"));
	static ConstructorHelpers::FClassFinder<UCameraShakeBase> FireCameraShake(TEXT("/Game/_Blueprints/FiringShake"));
	static ConstructorHelpers::FObjectFinder<USoundBase> GetWeaponRifleLowerSound(TEXT("/Game/_Sounds/Rifle/Wavs/Rifle_Lower"));
	static ConstructorHelpers::FObjectFinder<USoundBase> GetWeaponRifleRaiseSound(TEXT("/Game/_Sounds/Rifle/Wavs/Rifle_Raise"));
	static ConstructorHelpers::FObjectFinder<USoundBase> GetWeaponPistolLowerSound(TEXT("/Game/_Sounds/Pistol/Wavs/Pistol_Lower"));
	static ConstructorHelpers::FObjectFinder<USoundBase> GetWeaponPistolRaiseSound(TEXT("/Game/_Sounds/Pistol/Wavs/Pistol_Raise"));
	static ConstructorHelpers::FObjectFinder<USoundBase> GetFireModeChangeSound(TEXT("/Game/_Sounds/FireModeChange"));
	static ConstructorHelpers::FObjectFinder<USoundBase> GetWeaponRocketLauncherLowerSound(TEXT("/Game/_Sounds/RocketLauncher/Wavs/RocketLauncher_Lower"));
	static ConstructorHelpers::FObjectFinder<USoundBase> GetWeaponRocketLauncherRaiseSound(TEXT("/Game/_Sounds/RocketLauncher/Wavs/RocketLauncher_Raise"));
	static ConstructorHelpers::FObjectFinder<USkeletalMesh> GetSoldierSkeletalMesh(TEXT("/Game/_SoldierContents/SoldierMeshes/SoldierCharacter"));
	static ConstructorHelpers::FObjectFinder<UCurveFloat> Curve(TEXT("/Game/_Blueprints/_Curves/WeaponRecoilCurve"));
	if(WeaponInteractWidgetClass.Class && WeaponStatusWidgetClass.Class){
		TWeaponInteractWidget = WeaponInteractWidgetClass.Class;
		TWeaponStatusWidget = WeaponStatusWidgetClass.Class;
		WeaponRifleLowerSound = GetWeaponRifleLowerSound.Object;
		WeaponRifleRaiseSound = GetWeaponRifleRaiseSound.Object;
		WeaponPistolLowerSound = GetWeaponPistolLowerSound.Object;
		WeaponPistolRaiseSound = GetWeaponPistolRaiseSound.Object;
		WeaponRocketLauncherLowerSound = GetWeaponRocketLauncherLowerSound.Object;
		WeaponRocketLauncherRaiseSound = GetWeaponRocketLauncherRaiseSound.Object;
		CameraShake = FireCameraShake.Class;
		FireModeChangeSound = GetFireModeChangeSound.Object;
		GetMesh()->SetSkeletalMesh(GetSoldierSkeletalMesh.Object);
	}

	FOnTimelineFloat progressFunction;
	progressFunction.BindUFunction(this, "TimelineProgress");
	TimeLine.AddInterpFloat(Curve.Object, progressFunction);
}

void ASoldier::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent){
	if(PlayerInputComponent){  
		PlayerInputComponent->BindAction("Jump",IE_Pressed,this,&ACharacter::Jump);
		PlayerInputComponent->BindAction("Jump",IE_Released,this,&ACharacter::StopJumping);
		PlayerInputComponent->BindAction("Sprint",IE_Pressed,this,&ASoldier::Sprint);
		PlayerInputComponent->BindAction("Sprint",IE_Released,this,&ASoldier::StopSprint);
		PlayerInputComponent->BindAction("Crouch",IE_Pressed,this,&ASoldier::SoldierCrouch);
		PlayerInputComponent->BindAction("Equip",IE_Pressed,this,&ASoldier::EquipWeapons);
		PlayerInputComponent->BindAction("PrimaryWeapon", IE_Pressed, this, &ASoldier::PrimaryWeapon);
		PlayerInputComponent->BindAction("SecondaryWeapon", IE_Pressed, this, &ASoldier::SecondaryWeapon);
		PlayerInputComponent->BindAction("ThirdyWeapon", IE_Pressed, this, &ASoldier::ThirdyWeapon);
		PlayerInputComponent->BindAction("DropWeapon", IE_Pressed, this, &ASoldier::DropWeapon);
		PlayerInputComponent->BindAction("PutWeaponBack", IE_Pressed, this, &ASoldier::PutWeaponBackPressed);
		PlayerInputComponent->BindAction("Prone", IE_Pressed, this, &ASoldier::SoldierProne);
		PlayerInputComponent->BindAction("Fire", IE_Pressed, this, &ASoldier::SetFireMode);
		PlayerInputComponent->BindAction("Fire", IE_Released, this, &ASoldier::SetFireModeClear);
		PlayerInputComponent->BindAction("Aim", IE_Pressed, this, &ASoldier::AimSight);
		PlayerInputComponent->BindAction("FireMode", IE_Pressed, this, &ASoldier::FireMode);
		PlayerInputComponent->BindAction("AimDownSight", IE_Pressed, this, &ASoldier::AimDownSight);
		PlayerInputComponent->BindAction("TEST", IE_Pressed, this, &ASoldier::SoldierDeath);
		// PlayerInputComponent->BindAction("LookAround", IE_Pressed, this, &ASoldier::LookAroundHold);
		// PlayerInputComponent->BindAction("LookAround", IE_Released, this, &ASoldier::LookAroundReleased);

		PlayerInputComponent->BindAxis("MoveForward", this, &ASoldier::MoveForward);
		PlayerInputComponent->BindAxis("MoveRight", this, &ASoldier::MoveRight);

		PlayerInputComponent->BindAxis("Turn", this, &ASoldier::Turn);
		PlayerInputComponent->BindAxis("LookUp", this, &ASoldier::LookUp);
	}
}

void ASoldier::BeginPlay(){
	Super::BeginPlay();
	WeaponInteractWidget = CreateWidget<UWeaponInteractWidget>(this->GetWorld()->GetFirstPlayerController(),TWeaponInteractWidget);
	WeaponStatusWidget = CreateWidget<UWeaponStatusWidget>(this->GetWorld()->GetFirstPlayerController(), TWeaponStatusWidget);

	SelectItemCapsule->OnComponentBeginOverlap.AddDynamic(this, &ASoldier::SelectItemBeginOverlap);
	SelectItemCapsule->OnComponentEndOverlap.AddDynamic(this, &ASoldier::SelectItemEndOverlap);
	GetMesh()->OnComponentHit.AddDynamic(this, &ASoldier::BodyHit);
}

void ASoldier::EndPlay(EEndPlayReason::Type EndPlayReason){
	Super::EndPlay(EndPlayReason);
}

void ASoldier::Tick(float DeltaTime) {
    Super::Tick(DeltaTime);
	if(WeaponInteractWidget->IsValidLowLevel()){
		bool LineTrace = LineTraceCreate();
		if(LineTrace){
			IBaseWeaponInterface *SelectWeapon = Cast<IBaseWeaponInterface>(LineHitWeapon.GetActor());
			if (SelectWeapon != nullptr){
				if(!WeaponInteractWidget->IsInViewport()){
					CurrentLookAtWeapon = SelectWeapon->GetCurrentLookAtWeapon();
					USkeletalMeshComponent *SelectWeaponMesh = SelectWeapon->GetWeaponMesh();
					SelectWeaponMesh->SetRenderCustomDepth(true);
					bWeaponSelected = true;
					WeaponInteractWidget->AddToViewport();
				}
			}else{
				if(WeaponInteractWidget->IsInViewport()){
					bWeaponSelected = false;
					WeaponInteractWidget->RemoveFromParent();
				}
			}
		}else{
			if(WeaponInteractWidget->IsInViewport()){
				bWeaponSelected = false;
				WeaponInteractWidget->RemoveFromParent();
			}
		}
	}
}

void ASoldier::SoldierDeath(){
	TArray<USceneComponent*> GetAttachedWeapons;
	GetMesh()->GetChildrenComponents(false, GetAttachedWeapons);

	for(USceneComponent* i : GetAttachedWeapons){
		if(IBaseWeaponInterface *AttachedWeapon = Cast<IBaseWeaponInterface>(i->GetOwner())){
			AttachedWeapon->GetWeaponMesh()->GetOwner()->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
			AttachedWeapon->SetCollisionContainer();
			AttachedWeapon->GetWeaponMesh()->SetSimulatePhysics(true);
			RightHandIsValid = false;
			WeaponsStatusMap["ChildRifleActor"] = false;
			WeaponsStatusMap["ChildPistolActor"] = false;
			WeaponsStatusMap["ChildRocketLaunchActor"] = false;
			WeaponIsSwap = false;
		}
	}
	GetCapsuleComponent()->DestroyComponent();
	GetController()->UnPossess();
	GetMesh()->SetAllBodiesBelowSimulatePhysics(FName("Hips"),true);
	SetFireModeClear();
	WeaponInteractWidget->RemoveFromParent();
	if(WeaponStatusWidget->IsInViewport())
		WeaponStatusWidget->RemoveFromParent();
	SelectItemCapsule->SetActive(false);
	SelectItemCapsule->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	SoldierHealth = 0;
	bWeaponSelected = false;
	bSoldierIsDead = true;
	GetWorld()->GetTimerManager().SetTimer(RespawnHandle, this, &ASoldier::SoldierRespawn, 5.f, false);
}

void ASoldier::SoldierRespawn(){
    ACephedekiCevherGameMode* GameMode = Cast<ACephedekiCevherGameMode>(GetWorld()->GetAuthGameMode());
    if (GameMode){
		GameMode->RespawnPlayer(this);
	}
}

void ASoldier::BodyHit(UPrimitiveComponent *HitComp, AActor *OtherActor, UPrimitiveComponent *OtherComp, FVector NormalImpulse, const FHitResult &Hit){
	if(ABaseBullet *HitBullet = Cast<ABaseBullet>(OtherActor)){
		if(bSoldierIsDead){
			GetMesh()->SetAllBodiesSimulatePhysics(true);
			GetMesh()->AddImpulse(Hit.ImpactNormal * 5000.0f, Hit.BoneName);
		}
	}
}

void ASoldier::AimSight(){
	if(RightHandIsValid){
		if(!bIsAimSight){
			bIsAimSight = true;
			CameraBoom->SetRelativeLocation(FVector(-20.394497, 19.706797, 158.883240));
			CameraBoom->TargetArmLength = 58.630859;
			CameraBoom->SocketOffset = FVector(-5.677035, 22.466288, -9.668234);
			GetCharacterMovement()->bOrientRotationToMovement = false;
			GetCharacterMovement()->bUseControllerDesiredRotation = true;
		}
		else{
			bIsAimSight = false;
			CameraBoom->SetRelativeLocation(FVector(0.0,19.707230,166.882416));
			CameraBoom->TargetArmLength = 300.0f;
			CameraBoom->SocketOffset = FVector(0.0,22.466288,0.0);
			GetCharacterMovement()->bOrientRotationToMovement = true;
			GetCharacterMovement()->bUseControllerDesiredRotation = false;
			AimDownSight();
		}
	}
}

void ASoldier::AimDownSight(){
	if(RightHandIsValid){
		if(bIsAimSight){
			bIsAimSight = true;
			WeaponStatusWidget->WeaponCrosshair->SetVisibility(ESlateVisibility::Hidden);
			GetMesh()->HideBoneByName("Head",EPhysBodyOp::PBO_None);
			FollowCamera->AttachToComponent(CurrentRightHandWeapon->GetWeaponMesh(), FAttachmentTransformRules::SnapToTargetIncludingScale, "ADSocket");
			FollowCamera->SetFieldOfView(60);
			GetCharacterMovement()->bOrientRotationToMovement = false;
			GetCharacterMovement()->bUseControllerDesiredRotation = true;
		}else{
			WeaponStatusWidget->WeaponCrosshair->SetVisibility(ESlateVisibility::Visible);
			GetMesh()->UnHideBoneByName("Head");
			FollowCamera->DetachFromComponent(FDetachmentTransformRules::KeepWorldTransform);
			FollowCamera->AttachToComponent(CameraBoom, FAttachmentTransformRules::SnapToTargetIncludingScale);
			FollowCamera->SetFieldOfView(90);
			GetCharacterMovement()->bOrientRotationToMovement = true;
			GetCharacterMovement()->bUseControllerDesiredRotation = false;
			bIsAimSight = false;
		}
	}
}

void ASoldier::SoldierCrouch(){
	if(!bIsCrouch){
		float OutRadius;
		float OutHalfHeight;
		CameraBoom->SetRelativeLocation(FVector(-0.000267, 19.707144, 104.971191));
		GetCapsuleComponent()->GetScaledCapsuleSize(OutRadius, OutHalfHeight);
		GetMesh()->SetRelativeLocation(FVector(-15.12f, 0, ((OutHalfHeight / 1.5) * (-1))));
		GetCapsuleComponent()->SetCapsuleSize(29.47f, 57.0f);
		bIsCrouch = true;
		if (bIsSprint){
			GetCharacterMovement()->MaxWalkSpeed = 333.0f;
		}
	}else{
		CameraBoom->SetRelativeLocation(FVector(0.0,19.707230,166.882416));
		GetMesh()->SetRelativeLocation(FVector(0.0f, 0.0f, -91.08f));
		GetCapsuleComponent()->SetCapsuleSize(29.47f, 89.47f);
		bIsCrouch = false;
		if (!bIsSprint){
			GetCharacterMovement()->MaxWalkSpeed = 166.0f;
		}else{
			GetCharacterMovement()->MaxWalkSpeed = 500.0f;
		}
	}
}

void ASoldier::SoldierProne(){
	if(RightHandIsValid){
		if(!bIsProne){
			bIsProne = true;
			GetCharacterMovement()->MaxWalkSpeed = 30.0f;
		}else{
			bIsProne = false;
		}
	}
}

void ASoldier::Turn(float Rate){
	float turnBaseRate = Rate  * BaseTurnRate * GetWorld()->GetDeltaSeconds();
	AddControllerYawInput(turnBaseRate);
	
	FRotator DeltaRotator = GetBaseAimRotation() - GetActorRotation();
	DeltaRotator.Normalize();
	if(GetVelocity().Size() == 0){
		if(DeltaRotator.Yaw <= -70.f){
			bTurnRight = true;
			bTurnLeft = false;
		}else if (DeltaRotator.Yaw >= 70.f){
			bTurnRight = false;
			bTurnLeft = true;
		}else{
			bTurnRight = false;
			bTurnLeft = false;
		}
	}
}

void ASoldier::LookAroundHold(){
	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->bUseControllerDesiredRotation = false;

	// const FRotator DeltaRotator = GetBaseAimRotation() - GetActorRotation();
	// GEngine->AddOnScreenDebugMessage(-1, 2, FColor::Cyan, FString::Printf(TEXT("%f"), DeltaRotator.Yaw));
}

void ASoldier::LookAroundReleased(){
	GetCharacterMovement()->bOrientRotationToMovement = false;
	GetCharacterMovement()->bUseControllerDesiredRotation = true;
}

void ASoldier::LookUp(float Rate){
	float turnLookUpRate = Rate * BaseLookUpRate * GetWorld()->GetDeltaSeconds();
	AddControllerPitchInput(turnLookUpRate);
}

void ASoldier::MoveForward(float Value){
	if ((Controller != nullptr) && (Value != 0.0f)){
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		AddMovementInput(Direction, Value);
	}
}

void ASoldier::MoveRight(float Value){
	if ( (Controller != nullptr) && (Value != 0.0f) ){
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
		AddMovementInput(Direction, Value);
	}
}

void ASoldier::Sprint(){
	if(bIsCrouch){
		GetCharacterMovement()->MaxWalkSpeed = 333.0f;
	}
	if(bIsProne){
		GetCharacterMovement()->MaxWalkSpeed = 30.0f;
	}else{
		GetCharacterMovement()->MaxWalkSpeed = 500.0f;
	}
	bIsSprint = true;
}

void ASoldier::StopSprint(){
	GetCharacterMovement()->MaxWalkSpeed = 166.0f;
	bIsSprint = false;
}

void ASoldier::PrimaryWeapon(){
	if(WeaponsStatusMap["ChildRifleActor"]){
		CurrentWeaponSelected = 0;
		GetMesh()->GetAnimInstance()->Montage_Play(EquipRifleStandingAnimation);
		if(RightHandIsValid){
			WeaponIsSwap = true;
		}
	}
}

void ASoldier::SecondaryWeapon(){
	if(WeaponsStatusMap["ChildPistolActor"]){
		CurrentWeaponSelected = 1;
		GetMesh()->GetAnimInstance()->Montage_Play(EquipPistolStandingAnimation);
		if(RightHandIsValid){
			WeaponIsSwap = true;
		}
	}
}

void ASoldier::ThirdyWeapon(){
	if(WeaponsStatusMap["ChildRocketLaunchActor"]){
		CurrentWeaponSelected = 2;
		GetMesh()->GetAnimInstance()->Montage_Play(EquipRifleStandingAnimation);
		if(RightHandIsValid){
			WeaponIsSwap = true;
		}
	}
}

void ASoldier::EquipWeapons(){
	LineTraceCreate();
	if (IBaseWeaponInterface *Weapon = Cast<IBaseWeaponInterface>(LineHitWeapon.GetActor())){
		if(!WeaponsStatusMap["ChildRifleActor"] && Weapon->GetWeaponIsRifle()){
			print("Tufek Baglandi");
			Weapon->SetNoCollision();
			Weapon->GetWeaponMesh()->SetSimulatePhysics(false);
			Weapon->GetWeaponMesh()->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetIncludingScale, "rightBackSocket");
			WeaponsStatusMap["ChildRifleActor"] = true;
		}
		if(!WeaponsStatusMap["ChildPistolActor"] && Weapon->GetWeaponIsPistol()){
			Weapon->SetNoCollision();
			Weapon->GetWeaponMesh()->SetSimulatePhysics(false);
			Weapon->GetWeaponMesh()->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetIncludingScale, "pistolSideSocket");
			WeaponsStatusMap["ChildPistolActor"] = true;
		}
		if(!WeaponsStatusMap["ChildRocketLaunchActor"] && Weapon->GetWeaponIsRocketLauncher()){
			Weapon->SetNoCollision();
			Weapon->GetWeaponMesh()->SetSimulatePhysics(false);
			Weapon->GetWeaponMesh()->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetIncludingScale, "rocketLaunchBackSocket");
			WeaponsStatusMap["ChildRocketLaunchActor"] = true;
		}
	}
}

bool ASoldier::LineTraceCreate(const int16 &traceDistance, const float &Thickness, const FColor &lineColor){
	FHitResult hit;
	FVector start = FollowCamera->GetComponentLocation();
	FVector end = start + (FollowCamera->GetComponentRotation().Vector() * traceDistance);
	FCollisionQueryParams params;
	params.AddIgnoredActor(this);
	bool isLineTrace = GetWorld()->LineTraceSingleByObjectType(hit, start, end, FCollisionObjectQueryParams::AllObjects, params);
	LineHitWeapon = hit;
	return isLineTrace;
}

void ASoldier::DropWeapon(){
	GEngine->AddOnScreenDebugMessage(-1, 2, FColor::Yellow, FString::Printf(TEXT("%i"), CurrentWeaponSelected));
	if (RightHandIsValid && !bIsAimSight){
		const FVector CurrentLocation = (GetActorForwardVector() * 60) + GetActorLocation();
		const FRotator CameraRotation = FollowCamera->GetComponentRotation();

		switch (CurrentWeaponSelected){
			case EWeapons::Rifle:
				RightHandIsValid = false;
				WeaponsStatusMap["ChildRifleActor"] = false;
				WeaponIsSwap = false;
				CurrentRightHandWeapon->SetCollisionContainer();
				CurrentRightHandWeapon->GetWeaponMesh()->GetOwner()->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
				CurrentRightHandWeapon->GetWeaponMesh()->SetSimulatePhysics(true);
				CurrentRightHandWeapon->GetWeaponMesh()->AddForce(FollowCamera->GetForwardVector() * 35000);
				WeaponStatusWidget->RemoveFromParent();
				print("Tufek Atildi");
				break;
			
			case EWeapons::RocketLauncher:
				RightHandIsValid = false;
				WeaponsStatusMap["ChildRocketLaunchActor"] = false;
				WeaponIsSwap = false;
				CurrentRightHandWeapon->SetCollisionContainer();
				CurrentRightHandWeapon->GetWeaponMesh()->GetOwner()->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
				CurrentRightHandWeapon->GetWeaponMesh()->SetSimulatePhysics(true);
				CurrentRightHandWeapon->GetWeaponMesh()->AddForce(FollowCamera->GetForwardVector() * 35000);
				WeaponStatusWidget->RemoveFromParent();
				print("Rocket Launcher Atildi");
				break;

			case EWeapons::Pistol:
				RightHandIsValid = false;
				WeaponsStatusMap["ChildPistolActor"] = false;
				WeaponIsSwap = false;
				CurrentRightHandWeapon->SetCollisionContainer();
				CurrentRightHandWeapon->GetWeaponMesh()->GetOwner()->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
				CurrentRightHandWeapon->GetWeaponMesh()->SetSimulatePhysics(true);
				CurrentRightHandWeapon->GetWeaponMesh()->AddForce(FollowCamera->GetForwardVector() * 3500);
				WeaponStatusWidget->RemoveFromParent();
				break;
		}
	}
}

void ASoldier::SwapWeapons() {
    if (RightHandIsValid) {
        switch (CurrentWeaponSelected) {
            case EWeapons::Rifle:
                if (CurrentRightHandWeapon->GetWeaponIsPistol()) {
                    GEngine->AddOnScreenDebugMessage(-1, 3, FColor::Cyan, TEXT("Tabanca Tufekle yer degisti"));
                    CurrentRightHandWeapon->GetWeaponMesh()->GetOwner()->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
                    CurrentRightHandWeapon->GetWeaponMesh()->GetOwner()->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetIncludingScale, "pistolSideSocket");
                } else if (CurrentRightHandWeapon->GetWeaponIsRocketLauncher()) {
                    GEngine->AddOnScreenDebugMessage(-1, 3, FColor::Purple, TEXT("tüfek roketle yer değişti"));
                    CurrentRightHandWeapon->GetWeaponMesh()->GetOwner()->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
                    CurrentRightHandWeapon->GetWeaponMesh()->GetOwner()->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetIncludingScale, "rocketLaunchBackSocket");
                }
                break;

            case EWeapons::Pistol:
                if (CurrentRightHandWeapon->GetWeaponIsRifle()) {
                    GEngine->AddOnScreenDebugMessage(-1, 3, FColor::Emerald, TEXT("Tufek Tabancayla yer degisti"));
                    CurrentRightHandWeapon->GetWeaponMesh()->GetOwner()->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
                    CurrentRightHandWeapon->GetWeaponMesh()->GetOwner()->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetIncludingScale, "rightBackSocket");
                } else if (CurrentRightHandWeapon->GetWeaponIsRocketLauncher()) {
                    GEngine->AddOnScreenDebugMessage(-1, 3, FColor::Orange, TEXT("tabanca roketle yer değişti"));
                    CurrentRightHandWeapon->GetWeaponMesh()->GetOwner()->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
                    CurrentRightHandWeapon->GetWeaponMesh()->GetOwner()->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetIncludingScale, "rocketLaunchBackSocket");
                }
                break;
				
            case EWeapons::RocketLauncher:
                if (CurrentRightHandWeapon->GetWeaponIsRifle()) {
                    GEngine->AddOnScreenDebugMessage(-1, 3, FColor::Yellow, TEXT("Roket Tufekle yer degisti"));
                    CurrentRightHandWeapon->GetWeaponMesh()->GetOwner()->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
                    CurrentRightHandWeapon->GetWeaponMesh()->GetOwner()->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetIncludingScale, "rightBackSocket");
                } else if (CurrentRightHandWeapon->GetWeaponIsPistol()) {
                    GEngine->AddOnScreenDebugMessage(-1, 3, FColor::Magenta, TEXT("Roket Tabancayla yer degisti"));
                    CurrentRightHandWeapon->GetWeaponMesh()->GetOwner()->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
                    CurrentRightHandWeapon->GetWeaponMesh()->GetOwner()->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetIncludingScale, "pistolSideSocket");
                }
                break;
        }
    }
}

void ASoldier::PutWeaponBackPressed(){
	if(RightHandIsValid){
		PutWeaponPressedTime = GetWorld()->GetTimeSeconds();
		bPutWeaponIsReleased = false;
		GetWorldTimerManager().SetTimer(ReloadTimerHandle, this, &ASoldier::PutWeaponBackCheck, 0.1f, false);
	}
}

void ASoldier::PutWeaponBackCheck(){
	if(!bPutWeaponIsReleased)
		GetWorldTimerManager().SetTimer(PutWeaponIsReleasedLoopHandle, this, &ASoldier::PutWeaponIsReleasedLoop, 0.1f, true);
}

void ASoldier::PutWeaponIsReleasedLoop(){PutWeaponBackEnd();}

void ASoldier::PutWeaponBackEnd(){
	float ReloadEndTime = GetWorld()->GetTimeSeconds();
    float ReloadDuration = ReloadEndTime - PutWeaponPressedTime;
	ResultPutWeaponPressedTime = ReloadDuration;
	if(ResultPutWeaponPressedTime>=1.3f){
		switch (CurrentWeaponSelected){
			case EWeapons::Rifle:
				WeaponStatusWidget->RemoveFromParent();
				GetMesh()->GetAnimInstance()->Montage_Play(EquipRifleStandingAnimation);
				break;

			case EWeapons::Pistol:
				WeaponStatusWidget->RemoveFromParent();
				GetMesh()->GetAnimInstance()->Montage_Play(EquipPistolStandingAnimation);
				break;

			case EWeapons::RocketLauncher:
				WeaponStatusWidget->RemoveFromParent();
				GetMesh()->GetAnimInstance()->Montage_Play(EquipRifleStandingAnimation);
				break;
		}
		print("Silahi yerine koydun");
		bPutWeaponIsReleased = true;
		GetWorldTimerManager().ClearTimer(PutWeaponIsReleasedLoopHandle);
	}else{
		if(!GetWorld()->GetFirstPlayerController()->IsInputKeyDown(EKeys::R)){
			print("sarjor degisti");
			GetWorldTimerManager().ClearTimer(PutWeaponIsReleasedLoopHandle);
			WeaponReload();
		}
	}
}

void ASoldier::WeaponReload(){
	if (CurrentRightHandWeapon->GetMaxBullet() != 0){
		bReloading = true;
		GetMesh()->GetAnimInstance()->Montage_Play(RifleReloadStandingAnimation);
	}
}

void ASoldier::SelectItemBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) {
    if(bWeaponSelected)
		if (IBaseWeaponInterface* SelectWeapon = Cast<IBaseWeaponInterface>(OtherActor)) {
			SelectWeapon->GetWeaponMesh()->SetRenderCustomDepth(true);
		}
}

void ASoldier::SelectItemEndOverlap(UPrimitiveComponent *OverlappedComponent, AActor *OtherActor, UPrimitiveComponent *OtherComp, int32 OtherBodyIndex) {
	if (IBaseWeaponInterface *SelectWeapon = Cast<IBaseWeaponInterface>(OtherActor)){
		if(SelectWeapon){
			SelectWeapon->GetWeaponMesh()->SetRenderCustomDepth(false);
		}
	}
}

void ASoldier::TimelineProgress(float Value){
	AddControllerPitchInput(Value * 13);
	AddControllerYawInput(Value * (-3) );
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, FString::Printf(TEXT("timeline: %f value:%f"), TimeLine.GetPlaybackPosition(), Value));
}

void ASoldier::TickTimeline(){
	if (TimeLine.IsPlaying()){
		TimeLine.TickTimeline(DELTATIME);
	}else{
		if(bSoldierOnFire){
			TimeLine.PlayFromStart();
		}
		GetWorldTimerManager().ClearTimer(TimerHandle);
	}
}

void ASoldier::FireMode() {
    if (RightHandIsValid) {
        CurrentWeaponFireMode++;
        if (CurrentWeaponFireMode > 2) {
            CurrentWeaponFireMode = 0;
        }
		WeaponStatusWidget->RemoveFromParent();
		WeaponStatusWidget->AddToViewport();
		CurrentRightHandWeapon->SetCurrentWeaponFireMode(CurrentWeaponFireMode);
		UGameplayStatics::PlaySound2D(GetWorld(), FireModeChangeSound);
    }
}

void ASoldier::SetFireMode(){
	if(RightHandIsValid){
		GEngine->AddOnScreenDebugMessage(-1, 2, FColor::Red, FString::Printf(TEXT("%i"), CurrentWeaponFireMode));
		switch (CurrentRightHandWeapon->GetCurrentWeaponFireMode()){
			case EWeaponFireMode::Auto:
				if (CurrentRightHandWeapon->GetWeaponIsRifle()){
					print("Auto");
					GetWorld()->GetTimerManager().SetTimer(TH_RapidFaireHandle, this, &ASoldier::Fire, 0.107, true);
				}
				break;

			case EWeaponFireMode::Single:
				print("Single");
				GetWorld()->GetTimerManager().SetTimer(TH_RapidFaireHandle, this, &ASoldier::Fire, 0.03, false);
				GetWorldTimerManager().SetTimer(TimerHandle, this, &ASoldier::TickTimeline, 0.03, false, 0.0f);
				break;

			case EWeaponFireMode::SemiAuto:
				if (CurrentRightHandWeapon->GetWeaponIsRifle()){
					print("Semi-Auto");
					SemiAutoFireCount = 0;
					GetWorld()->GetTimerManager().SetTimer(TH_RapidFaireHandle, this, &ASoldier::SemiAutoFire, 0.08, true);
					GetWorldTimerManager().SetTimer(TimerHandle, this, &ASoldier::TickTimeline, 0.08, false, 0.0f);
				}
				break;
		}
	}
}

void ASoldier::SemiAutoFire(){
    if (SemiAutoFireCount < 2) {
		print("sATESSS");
		Fire(); // Ateşle
        SemiAutoFireCount++;
    } else {
        SemiAutoFireCount = 0; // Sayaçı sıfırla
        GetWorldTimerManager().ClearTimer(TH_RapidFaireHandle); // Zamanlayıcıyı kapat
    }
}

void ASoldier::SetFireModeClear(){
	TimeLine.SetTimelineLength(0);
	GetWorldTimerManager().ClearTimer(TimerHandle);
	GetWorldTimerManager().ClearTimer(TH_RapidFaireHandle);
}

void ASoldier::Fire(){
	if (RightHandIsValid){
		if(CurrentRightHandWeapon->GetCurrentBullet() != 0){
			TimeLine.Play();
			FHitResult ResultHit;
			const int32 length = 35000;
			const FVector Start = FollowCamera->GetComponentLocation();//CurrentRightHandWeapon->GetWeaponMesh()->GetSocketLocation(FName("Muzzle"));
			const FVector distance = FollowCamera->GetForwardVector() * length;
			const FVector End = Start + distance;

			FCollisionQueryParams QueryParams;
			QueryParams.AddIgnoredActor(this);
			bool trace = this->GetWorld()->LineTraceSingleByChannel(ResultHit, Start, End, ECollisionChannel::ECC_PhysicsBody, QueryParams);
			// DrawDebugLine(GetWorld(), Start, End, FColor::Cyan, false, 4);
			const FVector selectVector = SelectVector(ResultHit.ImpactPoint, ResultHit.TraceEnd, trace);
			const FRotator lookAtRot = FindLookAtRotation(CurrentRightHandWeapon->GetWeaponMesh()->GetSocketLocation(FName("Muzzle")), selectVector);
			const FRotator makeRotator = FRotator(lookAtRot.Pitch + 0.3, lookAtRot.Yaw, lookAtRot.Roll);
			const FTransform makeTransform = FTransform(makeRotator, CurrentRightHandWeapon->GetWeaponMesh()->GetSocketLocation(FName("Muzzle")),CurrentRightHandWeapon->GetWeaponBulletSize());
			FActorSpawnParameters params;
			params.Instigator = this;
			params.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
			const ABaseBullet *SpawnBullet = GetWorld()->SpawnActor<ABaseBullet>(CurrentRightHandWeapon->GetWeaponBulletClass(), makeTransform, params);
			if(SpawnBullet){
				bSoldierOnFire = true;
				if (bReloading)
					bullet = CurrentRightHandWeapon->GetCurrentBullet();
				bullet--;
				CurrentRightHandWeapon->SetCurrentBullet(bullet);
				GEngine->AddOnScreenDebugMessage(-1, 2, FColor::Red, FString::Printf(TEXT("%i"), CurrentRightHandWeapon->GetCurrentBullet()));
				const FTransform muzzleSocketTransform(CurrentRightHandWeapon->GetWeaponMesh()->GetSocketTransform(FName("Muzzle")));
				UGameplayStatics::SpawnEmitterAttached(CurrentRightHandWeapon->GetBulletMuzzleParticle(),CurrentRightHandWeapon->GetWeaponMesh(),"Muzzle",
													   muzzleSocketTransform.GetLocation(),FollowCamera->GetComponentRotation(),FVector(1,1,1),EAttachLocation::Type::KeepWorldPosition);
				UGameplayStatics::PlaySoundAtLocation(GetWorld(), CurrentRightHandWeapon->WeaponFireSound(), GetActorLocation(), 1, 1, 0,CurrentRightHandWeapon->WeaponFireSoundAttenuation());
				UGameplayStatics::PlayWorldCameraShake(GetWorld(), CameraShake,GetActorLocation(),10000,0);
				CurrentRightHandWeapon->GetWeaponMesh()->PlayAnimation(CurrentRightHandWeapon->GetWeaponFireAnimation(), false);
				GetWorldTimerManager().SetTimer(TimerHandle, this, &ASoldier::TickTimeline, 0.107, false, 0.0f);
				if(CurrentRightHandWeapon->GetWeaponIsRocketLauncher())
					UGameplayStatics::SpawnEmitterAttached(CurrentRightHandWeapon->GetRocketLauncherMuzzleRearParticle(),CurrentRightHandWeapon->GetWeaponMesh(),"MuzzleFlashRear",
					FTransform(CurrentRightHandWeapon->GetWeaponMesh()->GetSocketTransform(FName("MuzzleFlashRear"))).GetLocation(),FollowCamera->GetComponentRotation(),FVector(1,1,1),EAttachLocation::Type::KeepWorldPosition);
			}
		}else{
			UGameplayStatics::PlaySoundAtLocation(GetWorld(), CurrentRightHandWeapon->GetWeaponEmptyFireSound(), GetActorLocation(), 1, 1, 0);
			bSoldierOnFire = false;
		}

		if(bSoldierOnFire){
			GetMesh()->GetAnimInstance()->Montage_Play(FiringRifleAnimation);
		}
	}
}

FVector ASoldier::SelectVector(const FVector A, const FVector B, const bool bSelectA) const {
    return bSelectA ? A : B;
}

FRotator ASoldier::FindLookAtRotation(const FVector &Start, const FVector &Target){
    return FRotationMatrix::MakeFromX(Target - Start).Rotator();
}