#include "Character.h"

Character::Character(CharacterType type, string fileName, Vec2 pos, Size size)
{
    sprite = cocos2d::Sprite::create("Images/" + fileName);
    sprite->setPosition(pos);
    sprite->setContentSize(size);

    originPos = pos;

    this->type = type;

    switch (this->type)
    {
    case CharacterType::Player:
        dir = Dir::Right;
        break;
    case CharacterType::Enemy:
        dir = Dir::Left;
        break;
    }

    defaultTime = 0.5f;

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

void Character::Attack()
{
    Vec2 movingRange = Vec2((int)dir * 100, 0);

    auto frontMove = MoveTo::create(defaultTime, originPos + movingRange);
    auto comebackMove = MoveTo::create(defaultTime, originPos);

    auto moveSeq = Sequence::create(frontMove, comebackMove, nullptr);

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
