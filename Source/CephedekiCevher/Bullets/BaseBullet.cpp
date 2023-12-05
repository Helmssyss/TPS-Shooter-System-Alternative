#include "BaseBullet.h"

#include "UObject/ConstructorHelpers.h"
#include "Blueprint/UserWidget.h"

ABaseBullet::ABaseBullet(){
    static ConstructorHelpers::FClassFinder<UUserWidget> GetHitCrosshairWidget(TEXT("/Game/_Blueprints/_BP_Widgets/BP_HitCrosshairWidget"));
    TBulletWidget = GetHitCrosshairWidget.Class;
}