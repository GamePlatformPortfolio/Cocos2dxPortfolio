#include "CustomClasses/Character/Character.h"
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
			angle = -10; backangle = 10;
			imagesizeX =475; imagesizeY = 410; //sprite size
            Sound_PA = "Craver_PA.mp3";
            Sound_MA = "Craver_MA.mp3";
            break;
        }
        case CharType::ENEMY:
        {
            dir = Direction::LEFT;
			angle = 10; backangle = -10;
			imagesizeX = 800; imagesizeY = 400;
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
        Sound_Guard = "Sounds/DamageGuard.mp3";
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

void Character::Action(CharType targetType, Stone* curStone, DamageValue* damage)
{
	if (type == targetType) {
		if (damage->NpDamage() > 0) {
			if (currentHp - damage->HpDamage() > 0) sprite->setTexture("Images/" + GetSpriteName(type, CharAnim::DAMAGE_ANIM));

			else if (currentHp - damage->HpDamage() <= 0) sprite->setTexture("Images/" + GetSpriteName(type, CharAnim::DEAD_ANIM));
			sprite->setContentSize(Size(imagesizeX, imagesizeY));  

			Vec2 moveDistance = Vec2((int)dir * 100, 0);

			//actionTime = 0.5f;
			auto moveBack = MoveTo::create(0.08f, originPos - moveDistance);
			auto moveOrigin = MoveTo::create(0.3f, originPos);
			auto moveFront = MoveTo::create(0.06f, originPos + moveDistance);

			auto Blink = Blink::create(0.3f, 1);
			auto hitred = TintTo::create(0.08f, 255, 0, 0); 
			auto hitbakcred = TintTo::create(0.08f, 255, 255, 255); 
			auto RotatesBy = RotateBy::create(0.08f, angle);
			auto RotatesBack = RotateBy::create(0.08f, backangle);


			auto back = CallFunc::create([=]()->void {sprite->setTexture("Images/" + GetSpriteName(type, CharAnim::IDLE_ANIM));
			sprite->setContentSize(Size(imagesizeX, imagesizeY));    });

			auto movemotion = CallFunc::create([=]()->void {sprite->setTexture("Images/" + GetSpriteName(type, CharAnim::MOVE_ANIM));
				sprite->setContentSize(Size(imagesizeX, imagesizeY)); 
			});

			auto moveSeq = Sequence::create(
               movemotion, moveFront, back, DelayTime::create(0.5f), moveBack,
				Blink, RotatesBy, hitred, hitbakcred,  RotatesBack,
                moveOrigin, 
                back, 
                nullptr);

			AudioEngine::play2d(Sound_Hit, false, 0.1f);

			sprite->runAction(moveSeq);
            SufferDamage(damage);
            return;
		}
        SufferDamage(damage);
	}
	sprite->setTexture("Images/" + GetSpriteName(type, (CharAnim)curStone->GetType()));
	sprite->setContentSize(Size(imagesizeX, imagesizeY));

	Vec2 moveDistance = Vec2((int)dir * 100, 0);

	auto moveFront = MoveTo::create(actionTime, originPos + moveDistance);
	auto moveFront2 = MoveTo::create(0.06f, originPos + moveDistance);

	auto moveBack = MoveTo::create(actionTime, originPos - moveDistance);
	auto moveOrigin = MoveTo::create(actionTime, originPos);

	//motion
	auto back = CallFunc::create([=]()->void { sprite->setTexture("Images/" + GetSpriteName(type, CharAnim::IDLE_ANIM));
	sprite->setContentSize(Size(imagesizeX, imagesizeY));  
        });

	auto movemotion = CallFunc::create([=]()->void { sprite->setTexture("Images/" + GetSpriteName(type, CharAnim::MOVE_ANIM));
	sprite->setContentSize(Size(imagesizeX, imagesizeY));   
	});


	Sequence* moveSeq; 

	switch (curStone->GetType()) {
	case StoneType::PHYSICAL_ATTACK: {

		auto phyattack = CallFunc::create([=]()->void { sprite->setTexture("Images/" + GetSpriteName(type, CharAnim::PHYSICAL_ATTACK_ANIM));
		sprite->setContentSize(Size(imagesizeX, imagesizeY));  
		});

		AudioEngine::play2d(Sound_PA, false, 0.1f);
		moveSeq = Sequence::create(movemotion, moveFront2,  DelayTime::create(0.5f), phyattack, moveFront,
			DelayTime::create(0.2f),
			moveOrigin,
			back,
			nullptr);
	}break;
	case StoneType::MAGIC_ATTACK: { 
		auto magicmotion1 = CallFunc::create([=]()->void { sprite->setTexture("Images/" + GetSpriteName(type, CharAnim::MAGIC_ATTACK_ANIM_1));
		sprite->setContentSize(Size(imagesizeX, imagesizeY)); 
		});

		auto magicmotion2 = CallFunc::create([=]()->void { sprite->setTexture("Images/" + GetSpriteName(type, CharAnim::MAGIC_ATTACK_ANIM_2));
		sprite->setContentSize(Size(imagesizeX, imagesizeY));  
		});

		AudioEngine::play2d(Sound_MA, false, 0.1f);
		moveSeq = Sequence::create(movemotion, moveFront2,  DelayTime::create(0.5f), magicmotion1,  magicmotion2, moveFront,
			DelayTime::create(0.2f),
			moveOrigin, back,
			nullptr);

	}break;
	case StoneType::GUARD: { 
		auto guardmotion = CallFunc::create([=]()->void { sprite->setTexture("Images/" + GetSpriteName(type, CharAnim::GUARD_ANIM));
		sprite->setContentSize(Size(imagesizeX, imagesizeY));   
		});

		AudioEngine::play2d(Sound_Guard, false, 0.1f);
		moveSeq = Sequence::create(movemotion, moveFront2, DelayTime::create(0.5f), guardmotion ,moveOrigin,
			DelayTime::create(0.2f),
			moveOrigin, back,
			nullptr);

	}break;
	case StoneType::DODGE: { 
		auto dodgemotion = CallFunc::create([=]()->void {
			sprite->setTexture("Images/" + GetSpriteName(type, CharAnim::DODGE_ANIM));
			sprite->setContentSize(Size(imagesizeX, imagesizeY)); 
		});

		AudioEngine::play2d(Sound_DG, false, 0.1f);
		moveSeq = Sequence::create(movemotion, moveFront2,  DelayTime::create(0.5f), dodgemotion, moveBack,
			DelayTime::create(0.2f),
			moveOrigin, back,
			nullptr);

	}break;
	}
	
    sprite->runAction(moveSeq);
}

void Character::SufferDamage(DamageValue* damage){
#pragma region Animation

#pragma endregion
#pragma region DamageAffection
    currentHp -= damage->HpDamage(0);
    currentNp -= damage->NpDamage(0);

	stat->SetHpGauge(maxHp, currentHp);
	stat->SetNpGauge(maxNp, currentNp);
#pragma endregion
}

string Character::GetSpriteName(CharType name, CharAnim anim)
{
    string targetName = "";
	int imagesize;
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
		//add
	case MOVE_ANIM: targetAnim = "Move";  break; 
    case PHYSICAL_ATTACK_ANIM: targetAnim = "PA"; break;
	case MAGIC_ATTACK_ANIM_1: targetAnim = "MA_01";  break; 
	case MAGIC_ATTACK_ANIM_2: targetAnim = "MA_02";  break; 
    case GUARD_ANIM: targetAnim = "GD"; break;
    case DODGE_ANIM: targetAnim = "DG"; break;
    case DAMAGE_ANIM: targetAnim = "Hit"; break;
    case IDLE_ANIM: targetAnim = "Standing"; break;
    case DEAD_ANIM: targetAnim = "Die"; break;
    } 

    return targetName + targetAnim + ".png";
}


