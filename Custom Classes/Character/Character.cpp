#include "Custom Classes/Stone/Stone.h"
#include "Character.h"
#include <AudioEngine.h>


Character::Character(CharacterType type, string fileName, Vec2 pos, Size size)
{
    sprite = cocos2d::Sprite::create(fileName);//"Images/" + 
    sprite->setPosition(pos);
    sprite->setContentSize(size);

    originPos = pos;

    this->type = type;

    //Set Character's Direction and Sound Effects
    switch (this->type)
    {
    case CharacterType::Player: {
        dir = Dir::Right;
        Sound_PA = "Craver_PA.mp3";
        Sound_MA = "Craver_MA.mp3";
        break;
    }
    case CharacterType::Enemy: {
        dir = Dir::Left;
        Sound_PA = "Guardian_PA.mp3";
        Sound_MA = "Guardian_MA.mp3";
        break;
    }
    }

    //Set Common Sound Effects
    {
        Sound_DG = "Dodge.mp3";
        Sound_AD = "Attack_Draw.mp3";
        Sound_Hit = "Hit.mp3";
    }

    defaultTime = 0.2f;

    maxHp = 10;
    currentHp = maxHp;
}

Character::~Character()
{

}

Sprite* Character::GetSprite()
{
    return sprite;
}

CharacterType Character::GetType()
{
    return type;
}

Dir Character::GetDir()
{
    return dir;
}

Vec2 Character::GetOriginPos()
{
    return originPos;
}

void Character::Action(Stone* curStone) {

}

void Character::Attack(Stone* curStone)
{
    Vec2 movingRange = Vec2((int)dir * 100, 0);

    auto frontMove = MoveTo::create(defaultTime, originPos + movingRange);
    auto comebackMove = MoveTo::create(defaultTime, originPos);

    auto moveSeq = Sequence::create(frontMove, DelayTime::create(0.2f), comebackMove, nullptr);


    switch (curStone->GetType()) {
    case StoneType::PhysicalAttack : AudioEngine::play2d(Sound_PA, false, 0.1f); break;
    case StoneType::MagicAttack    : AudioEngine::play2d(Sound_MA, false, 0.1f); break;
    }

    sprite->runAction(moveSeq);
}

void Character::Damaged(int value)
{
    if (value >= 0)
        currentHp -= value;

    Vec2 movingRange = Vec2((int)dir * -1 * 100, 0);

    auto backMove = MoveTo::create(defaultTime, originPos + movingRange);
    auto comebackMove = MoveTo::create(defaultTime, originPos);

    auto moveSeq = Sequence::create(DelayTime::create(defaultTime), backMove, comebackMove, nullptr);
    AudioEngine::play2d(Sound_Hit, false, 0.1f);

    sprite->runAction(moveSeq);
}

void Character::Hide()
{
    auto fadeOut = FadeOut::create(defaultTime);

    sprite->runAction(fadeOut);
}

void Character::Show()
{
    auto fadeIn = FadeIn::create(defaultTime);

    sprite->runAction(fadeIn);
}

int Character::GetMaxHp()
{
    return maxHp;
}

int Character::GetCurrentHp()
{
    return currentHp;
}

