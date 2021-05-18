#pragma once

#include "Custom Classes/Enum Collection/EnumCollection.h"
#include "Custom Classes/Stone/Stone.h"
#include "cocos2d.h"

using namespace cocos2d;
using namespace std;
using namespace cocos2d::experimental;

class Character
{
public:
    Character(CharacterType type, string fileName, Vec2 pos, Size size);
    ~Character();

    Sprite*         GetSprite();
    CharacterType   GetType();
    Dir             GetDir();

    Vec2            GetOriginPos();

    void            Action(Stone* curStone);
    void            Attack(Stone* curStone);
    void            Damaged(int value);

    void            Hide();
    void            Show();

    int             GetMaxHp();
    int             GetCurrentHp();

private:
    Sprite*         sprite;      // 읽기 전용
    CharacterType   type;        // 읽기 전용
    Dir             dir;         // 읽기 전용
                                         
    Vec2            originPos;   // 읽기 전용
                                         
    float           defaultTime;         
    int             maxHp;       // 읽기 전용 
    int             currentHp;           
    int             maxNp;       // 읽기 전용
    int             currentNp;           
    int             maxEp;       // 읽기 전용
    int             currentEp;
    int             regenEp;     // 읽기 전용

    string Sound_PA;            //Sound of Physical Attack
    string Sound_MA;            //Sound of Magic Attack

    string Sound_DG;            //Sound of Dodge
    string Sound_AD;            //Attack Draw
    string Sound_Hit;


};
