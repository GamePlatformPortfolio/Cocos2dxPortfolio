#include "Custom Classes/Character/Character.h"
#include <AudioEngine.h>

Character::Character(CharType type, string root, Vec2 pos, Size size)
{
    //Set Character's Sprite
    sprite = Sprite::create(root + GetSpriteName(type, CharAnim::IDLE_ANIM));

    sprite->setPosition(pos);
    sprite->setContentSize(size);

    this->type = type;

    maxHp = 10; currentHp = maxHp;
    maxEp = 20; currentEp = maxEp;
    maxNp = 10; currentNp = maxNp;
    regenEp = 5;

    this->originPos = pos;

    //Set Character's Sound Effects
    switch (this->type)
    {
        case CharType::PLAYER:
        {
            dir = Direction::RIGHT;
            Sound_PA = "Craver_PA.mp3";
            Sound_MA = "Craver_MA.mp3";
            break;
        }
        case CharType::ENEMY:
        {
            dir = Direction::LEFT;
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

    stat = new StatPanel(type, root);
    sprite->addChild(stat->GetBackGround());
    stat->GetBackGround()->setPosition(Vec2(sprite->getContentSize().width/2, sprite->getContentSize().height / 2 -100));
    sprite->addChild(stat->GetHpGauge());
    stat->GetHpGauge()->setAnchorPoint(Vec2(1, 0.5));
    stat->GetHpGauge()->setPosition(Vec2(stat->GetBackGround()->getPosition().x, stat->GetBackGround()->getPosition().y));
    sprite->addChild(stat->GetNpGauge());
    stat->GetNpGauge()->setPosition(Vec2(sprite->getContentSize().width/2 + 50, sprite->getContentSize().height / 2 -100));
}

Character::~Character()
{

}

Sprite* Character::GetSprite()
{
    return sprite;
}

CharType Character::GetType()
{
    return type;
}

void Character::Hide()
{
    auto fadeOut = FadeOut::create(actionTime);

    sprite->runAction(fadeOut);
}

void Character::Show()
{
    auto fadeIn = FadeIn::create(actionTime);

    sprite->runAction(fadeIn);
}

void Character::Action(Stone* curStone)
{
    sprite->setTexture("Images/" + GetSpriteName(type, (CharAnim)curStone->GetType()));
    sprite->setContentSize(Size(300, 300));

    Vec2 moveDistance = Vec2((int)dir * 100, 0);


    auto moveFront = MoveTo::create(actionTime, originPos + moveDistance);
    auto moveBack = MoveTo::create(actionTime, originPos - moveDistance);
    auto moveOrigin = MoveTo::create(actionTime, originPos);

    auto back = CallFunc::create([=]()->void {sprite->setTexture("Images/" + GetSpriteName(type, CharAnim::IDLE_ANIM));
    sprite->setContentSize(Size(300, 300)); });

    auto moveSeq = Sequence::create(moveFront, DelayTime::create(0.2f), moveOrigin, back, nullptr);

    switch (curStone->GetType()) 
    {
    case StoneType::PHYSICAL_ATTACK : AudioEngine::play2d(Sound_PA, false, 0.1f); break;
    case StoneType::MAGIC_ATTACK    : AudioEngine::play2d(Sound_MA, false, 0.1f); break;
    }

    sprite->runAction(moveSeq);
}

void Character::SufferDamage(int value)
{
    if (value < 0) return;

    if(currentHp - value > 0) sprite->setTexture("Images/" + GetSpriteName(type, CharAnim::DAMAGE_ANIM));
    else if(currentHp - value <= 0) sprite->setTexture("Images/" + GetSpriteName(type, CharAnim::DEAD_ANIM));
    sprite->setContentSize(Size(300, 300));

#pragma region Animation
    Vec2 moveDistance = Vec2((int)dir * 100, 0);

    auto moveFront = MoveTo::create(actionTime, originPos + moveDistance);
    auto moveBack = MoveTo::create(actionTime, originPos - moveDistance);
    auto moveOrigin = MoveTo::create(actionTime, originPos);

    auto back = CallFunc::create([=]()->void {sprite->setTexture("Images/" + GetSpriteName(type, CharAnim::IDLE_ANIM));
    sprite->setContentSize(Size(300, 300)); });

    auto moveSeq = Sequence::create(DelayTime::create(actionTime), moveBack, moveOrigin, back, nullptr);
    AudioEngine::play2d(Sound_Hit, false, 0.1f);

    sprite->runAction(moveSeq);
#pragma endregion
#pragma region MyRegion
    currentHp -= value;

    //Getint if hostileType is Physical of magic attack
    //if(hostileType < 2) { currentHp -= value;}
    currentNp -= value/2;

	stat->SetHpGauge(maxHp, currentHp);
	stat->SetNpGauge(maxNp, currentNp);
#pragma endregion
}

string Character::GetSpriteName(CharType name, CharAnim anim)
{
    string targetName = "";
    switch (name)
    {
    case CharType::PLAYER:
        targetName = "Craver/Craver_";
        break;
    case CharType::ENEMY:
        targetName = "Guardian/Guardian_";
        break;
    }

    string targetAnim = "";
    switch (anim)
    {
    case PHYSICAL_ATTACK_ANIM: targetAnim = "PA"; break;
    case MAGIC_ATTACK_ANIM: targetAnim = "MA";  break;
    case GUARD_ANIM: targetAnim = "GD"; break;
    case DODGE_ANIM: targetAnim = "DG"; break;
    case DAMAGE_ANIM: targetAnim = "Hit"; break;
    case IDLE_ANIM: targetAnim = "Standing"; break;
    case DEAD_ANIM: break;
    } 


    return targetName + targetAnim + ".png";
}