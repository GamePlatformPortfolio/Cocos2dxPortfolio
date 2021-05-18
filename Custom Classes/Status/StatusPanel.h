#pragma once

#include "Custom Classes/Enum Collection/EnumCollection.h"
#include "Custom Classes/Character/Character.h"
#include "cocos2d.h"
#include "ui/CocosGUI.h"

using namespace std;
using namespace cocos2d;
using namespace ui;

class StatusPanel
{
public:
    Sprite*         background;
    //LoadingBar*         hpBar;
    Sprite*         hpBar;

    StatusPanel(string bgFileName, string hpFileName,
        Character* character, Size size, Vec2 distance, int hp);
    ~StatusPanel();

    int             GetMaxHp();
    int             GetHp();
    void            SetHp(int value);
    void            UpdateHpBar();
    void            AttachToCharacter(Vec2 distance);

    CharacterType   GetTarget();
    Character* target;

private:
    Size            bgSize;
    Vec2            distanceWithCharacter;
    Size            originSize;


    int             maxHp;
    int             currentHp;
};
