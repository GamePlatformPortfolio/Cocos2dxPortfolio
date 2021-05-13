#pragma once

#include "Custom Classes/Enum Collection/EnumCollection.h"
#include "cocos2d.h"

using namespace cocos2d;
using namespace std;

class Character
{
public:
    Character(CharacterType type, string fileName, Vec2 pos, Size size);
    ~Character();

    Sprite*         GetSprite();
    CharacterType   GetType();
    Dir             GetDir();

    Vec2            GetOriginPos();

    void            Attack();
    void            Damaged(int value);

    void            Hide();
    void            Show();

    int             GetMaxHp();
    int             GetCurrentHp();

private:
    Sprite*         sprite;      // 읽기전용
    CharacterType   type;        // 읽기전용
    Dir             dir;         // 읽기전용

    Vec2            originPos;   // 읽기전용

    float           defaultTime;
    int             maxHp;       // 읽기전용 
    int             currentHp;
    int             maxNp;       // 읽기전용
    int             currentNp;
};
