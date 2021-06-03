#pragma once

#include "Custom Classes/Enum Collection/EnumCollection.h"
#include "Custom Classes/Stone/Stone.h"
#include "cocos2d.h"

using namespace cocos2d;
using namespace cocos2d::experimental;
using namespace std;

class Character
{
public:
    Character(CharType type, string root, Vec2 pos, Size size);
    ~Character();

    Sprite* GetSprite();
    CharType GetType();

    void Hide();
    void Show();

    void Attack(Scene* scene, Stone* curStone);
    void SufferDamage(int value);

    const int GetMaxHp();
    const int GetCurrentHp();

    string GetSpriteName(CharType type, CharAnim anim);
    float noise(int x, int y);
    void ChangeState(CharState state);

    void CameraShake(Scene* scene);
#pragma region PRIVATE
private:
    Sprite* sprite;
    CharType type;
    Direction dir;
    Vec2 originPos;

    const float actionTime = 0.5f;

    CharState state;

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
