#pragma once

#include "Custom Classes/StatPanel/StatPanel.h"
#include "Custom Classes/Enum Collection/EnumCollection.h"
#include "Custom Classes/Stone/Stone.h"
#include "cocos2d.h"

using namespace cocos2d;
using namespace cocos2d::experimental;
using namespace std;
class StatPanel;

class Character
{
public:
    Character(CharType type, string root, Vec2 pos, Size size);
    ~Character();

    Sprite* GetSprite();
    CharType GetType();

    void Hide();
    void Show();

    void Action(Stone* curStone);
    void SufferDamage(int value);
    void RegenEp() {currentEp += regenEp;}

    int GetMaxHp()        {return maxHp;}
    int GetCurrentHp()    {return currentHp;}
    int GetMaxNp()        {return maxNp;}
    int GetCurrentNp()    {return currentNp;}
    int GetMaxEp()        {return maxEp;}
    int GetCurrentEp()    {return currentEp;}

    void UseEp(int epUsed) {currentEp -= epUsed;}

    string GetSpriteName(CharType type, CharAnim anim);

#pragma region PRIVATE
private:
    Sprite* sprite;
    CharType type;
    Direction dir;
    Vec2 originPos;

    //friend class StatPanel;
    StatPanel* stat;
    //EpPanel* epDisplay;


    const float actionTime = 0.5f;

    int maxHp;
    int currentHp;
    int maxNp;
    int currentNp;
    int maxEp;
    int currentEp;
    int regenEp;

    string Sound_PA; //Sound of Physical Attack
    string Sound_MA; //Sound of Magic Attack

    string Sound_DG; //Sound of Dodge
    string Sound_AD; //Attack Draw
    string Sound_Hit;
#pragma endregion
};
