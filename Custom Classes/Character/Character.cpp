#include "Character.h"
#include <AudioEngine.h>
#include "HelloWorldScene.h"

Character::Character(CharType type, string root, Vec2 pos, Size size)
{
    //Set Character's Sprite
    sprite = Sprite::create(root + GetSpriteName(type, CharAnim::IDLE_ANIM));

    sprite->setPosition(pos);
    sprite->setContentSize(size);

    this->type = type;
    this->maxHp = 10;
    this->maxEp = 10;
    this->maxNp = 10;

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

void Character::Attack(Scene* scene, Stone* curStone)
{
    ChangeState(CharState::ATTACK);

    sprite->setTexture("Images/" + GetSpriteName(type, CharAnim::PHYSICAL_ATTACK_ANIM));
    sprite->setContentSize(Size(300, 300));

    Vec2 moveDistance = Vec2((int)dir * 100, 0);

    auto moveFront = MoveTo::create(actionTime, originPos + moveDistance);
    auto moveOrigin = MoveTo::create(actionTime, originPos);

    auto back = CallFunc::create([=]()->void {sprite->setTexture("Images/" + GetSpriteName(type, CharAnim::IDLE_ANIM));
    sprite->setContentSize(Size(300, 300));  });

    auto shake = CallFunc::create([=]()->void { CameraShake(scene); });

    auto moveSeq = Sequence::create(moveFront, shake, DelayTime::create(0.2f), moveOrigin, back, nullptr);

    switch (curStone->type) 
    {
    case StoneType::PHYSICAL_ATTACK : AudioEngine::play2d(Sound_PA, false, 0.1f); break;
    case StoneType::MAGIC_ATTACK    : AudioEngine::play2d(Sound_MA, false, 0.1f); break;
    }

    sprite->runAction(moveSeq);
}

void Character::SufferDamage(int value)
{
    if (value < 0)
        return;

    ChangeState(CharState::DAMAGE);

    sprite->setTexture("Images/" + GetSpriteName(type, CharAnim::DAMAGE_ANIM));
    sprite->setContentSize(Size(300, 300));

    currentHp -= value;

    Vec2 moveDistance = Vec2((int)dir * 100, 0);

    auto moveBack = MoveTo::create(actionTime, originPos - moveDistance);
    auto moveOrigin = MoveTo::create(actionTime, originPos);

    auto back = CallFunc::create([=]()->void {sprite->setTexture("Images/" + GetSpriteName(type, CharAnim::IDLE_ANIM));
    sprite->setContentSize(Size(300, 300));  });

    auto moveSeq = Sequence::create(DelayTime::create(actionTime), moveBack, moveOrigin, back, nullptr);
    AudioEngine::play2d(Sound_Hit, false, 0.1f);

    sprite->runAction(moveSeq);

}

const int Character::GetMaxHp()
{
    return maxHp;
}

const int Character::GetCurrentHp()
{
    return currentHp;
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
    case IDLE_ANIM:
        targetAnim = "Standing";
        break;
    case PHYSICAL_ATTACK_ANIM:
        targetAnim = "PA";
        break;
    case MAGIC_ATTACK_ANIM:
        targetAnim = "MA";
        break;
    case DAMAGE_ANIM:
        targetAnim = "Hit";
        break;
    case DEAD_ANIM:
        // º¸·ù
        break;
    } 


    return targetName + targetAnim + ".png";
}

void Character::ChangeState(CharState state)
{
    this->state = state;

    sprite->stopAllActions();

    if (this->state == CharState::IDLE)
    {
        auto upMove = MoveBy::create(0.5f, Vec2(0,   5));
        auto downMove = MoveBy::create(0.5f, Vec2(0, -5  ));

        auto moveSeq = Sequence::create(upMove, downMove, NULL);
        auto repeatSeq = RepeatForever::create(moveSeq);
        sprite->runAction(repeatSeq);
    }
}

float Character::noise(int x, int y) {
    int n = x + y * 57;
    n = (n << 13) ^ n;
    return (1.0 - ((n * ((n * n * 15731) + 789221) + 1376312589) & 0x7fffffff) / 1073741824.0);
}

void Character::CameraShake(Scene* scene)
{
    // experiment more with these four values until you rest with something you like!
    float interval = 0.f;
    float duration = 0.5f;
    float speed = 2.0f;
    float magnitude = 1.0f;

    static float elapsed = 0.f;

    scene->schedule([=](float dt) {
        float randomStart = random(-1000.0f, 1000.0f); 
            elapsed += dt;

        float percentComplete = elapsed / duration;

        // We want to reduce the shake from full power to 0 starting half way through
        float damper = 1.0f - clampf(2.0f * percentComplete - 1.0f, 0.0f, 1.0f);

        // Calculate the noise parameter starting randomly and going as fast as speed allows
        float alpha = randomStart + speed * percentComplete;

        // map noise to [-1, 1]
        float x = noise(alpha, 0.0f) * 2.0f - 1.0f;
        float y = noise(0.0f, alpha) * 2.0f - 1.0f;

        x *= magnitude * damper;
        y *= magnitude * damper;
        scene->setPosition(x, y);

        if (elapsed >= duration)
        {
            elapsed = 0;
            scene->unschedule("Shake");
            scene->setPosition(Vec2::ZERO);
        }

        }, interval, CC_REPEAT_FOREVER, 0.f, "Shake");
}

