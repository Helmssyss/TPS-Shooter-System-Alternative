#include "BaseMagazine.h"

#include "Sound/SoundBase.h"

#include "UObject/ConstructorHelpers.h"

ABaseMagazine::ABaseMagazine(){
    static ConstructorHelpers::FObjectFinder<USoundBase> GetRifleReloadSound01(TEXT("/Game/_Sounds/Rifle/Wavs/Rifle_Reload01"));
    static ConstructorHelpers::FObjectFinder<USoundBase> GetRifleReloadSound02(TEXT("/Game/_Sounds/Rifle/Wavs/Rifle_Reload02"));
    static ConstructorHelpers::FObjectFinder<USoundBase> GetRifleReloadSound03(TEXT("/Game/_Sounds/Rifle/Wavs/Rifle_Reload03"));
    static ConstructorHelpers::FObjectFinder<USoundBase> GetPistolReloadSlideSound(TEXT("/Game/_Sounds/Pistol/Wavs/Pistol_ReloadSlideRack01"));

    static ConstructorHelpers::FObjectFinder<USoundBase> GetPistolEjectMagSound(TEXT("/Game/_Sounds/Pistol/Cues/Pistol_ReloadEject_Cue"));

    ReloadSound01 = GetRifleReloadSound01.Object;
    ReloadSound02 = GetRifleReloadSound02.Object;
    ReloadSound03 = GetRifleReloadSound03.Object;

    PistolEjectMagSound = GetPistolEjectMagSound.Object;
    PistolReloadSlideSound = GetPistolReloadSlideSound.Object;
}