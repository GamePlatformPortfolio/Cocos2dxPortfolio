#include "HelloWorldScene.h"
#include "SimpleAudioEngine.h"
#include "AudioEngine.h"
#include <random>

using namespace cocos2d::experimental;

USING_NS_CC;

Scene* HelloWorld::createScene()
{
    return HelloWorld::create();
}

static void problemLoading(const char* filename)
{
    printf("Error while loading: %s\n", filename);
    printf("Depending on how you compiled you might have to add 'Resources/' in front of filenames in HelloWorldScene.cpp\n");
}

bool HelloWorld::init()
{
    if ( !Scene::init() )
    {
        return false;
    }

    // 스톤의 랜덤값을 위해 필요합니다.
    srand((unsigned int)time(NULL));


    InitCocosContent();

    InitStonePanel();
    InitCharacter();

    player->ChangeState(CharState::IDLE);
    enemy->ChangeState(CharState::IDLE);

    scheduleUpdate();

    return true;
}

void HelloWorld::InitStonePanel()
{
    playerStone = new StonePanel(
        CharType::PLAYER,
        SPRITE_ROOT_FOLDER,
        Vec2(GetScreenSize().width / 2, GetScreenSize().height / 8),
        Size(GetScreenSize().width, GetScreenSize().height / 4)
    );
    this->addChild(playerStone->panelSprite);

    enemyStone = new StonePanel(
        CharType::ENEMY,
        SPRITE_ROOT_FOLDER,
        Vec2(GetScreenSize().width / 2, GetScreenSize().height - GetScreenSize().height / 8),
        Size(GetScreenSize().width, GetScreenSize().height / 4)
    );
    this->addChild(enemyStone->panelSprite);
}

void HelloWorld::InitCharacter()
{
    player = new Character(
        CharType::PLAYER,
        SPRITE_ROOT_FOLDER,
        Vec2(GetScreenMiddlePos() - Vec2(GetScreenSize().width / 4, 0)),
        Size(300, 300)
    );
    this->addChild(player->GetSprite());

    enemy = new Character(
        CharType::ENEMY,
        SPRITE_ROOT_FOLDER,
        Vec2(GetScreenMiddlePos() + Vec2(GetScreenSize().width / 4, 0)),
        Size(300, 300)
    );
    this->addChild(enemy->GetSprite());
}

void HelloWorld::InitCocosContent()
{
    string bgmFilePath = "BGM.mp3";
    AudioEngine::play2d(AUDIO_ROOT_FOLDER + bgmFilePath, true, 0.1f);

    string bgImgFilePath = "Scene/BattleScene/Background.png";
    bgSprite = Sprite::create(SPRITE_ROOT_FOLDER + bgImgFilePath);
    bgSprite->setPosition(GetScreenMiddlePos());
    bgSprite->setContentSize(GetScreenSize());

    this->addChild(bgSprite);

    battleMenuItem = MenuItemFont::create("Battle", CC_CALLBACK_1(HelloWorld::StartBattle, this));
    battleMenuItem->setColor(Color3B::BLACK);

    battleMenu = Menu::create(battleMenuItem, nullptr);
    battleMenu->alignItemsHorizontally();

    this->addChild(battleMenu);
}

void HelloWorld::onEnter()
{
    Scene::onEnter();

    listener = EventListenerTouchOneByOne::create();

    listener->setSwallowTouches(true);

    listener->onTouchBegan      = CC_CALLBACK_2(HelloWorld::onTouchBegan, this);
    listener->onTouchMoved      = CC_CALLBACK_2(HelloWorld::onTouchMoved, this);
    listener->onTouchEnded      = CC_CALLBACK_2(HelloWorld::onTouchEnded, this);
    listener->onTouchCancelled  = CC_CALLBACK_2(HelloWorld::onTouchCancelled, this);

    _eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);
}

void HelloWorld::onExit()
{
    _eventDispatcher->removeEventListener(listener);

    Scene::onExit();
}

bool HelloWorld::onTouchBegan(Touch* touch, Event* event)
{        
    if (battleState == BattleState::BATTLE)
        return true;

    auto touchPoint = touch->getLocation();

    for (int i = 0; i < handAmount; i++)
    {
        bool isTouch = playerStone->handStones[i]->stoneImg->getBoundingBox().containsPoint(touchPoint);
        if (isTouch)
        {
            if (playerStone->handStones[i]->state == StoneState::SELECTED)
            {
                playerStone->UnSelectedStone(i);
            }
            else if (!playerStone->handStones[i]->state == StoneState::SELECTED && canSelectStone)
            {
                playerStone->SelectStone(i);
            }

            playerStone->handStones[i]->stoneInformImg->setVisible(true);
        }
    }
    return true;
}

void HelloWorld::onTouchMoved(Touch* touch, Event* event)
{
    auto touchPoint = touch->getLocation();
}

void HelloWorld::onTouchEnded(Touch* touch, Event* event)
{
    auto touchPoint = touch->getLocation();

    for (int i = 0; i < handAmount; i++)
    {

        playerStone->handStones[i]->stoneInformImg->setVisible(false);
    }
}

void HelloWorld::onTouchCancelled(Touch* touch, Event* event)
{
    auto touchPoint = touch->getLocation();
}

void HelloWorld::update(float dt)
{
    if (battleState == BattleState::BATTLE)
    {
        battleMenu->setVisible(false);
        canSelectStone = false;
        return;
    }
       

    int stoneAmount = playerStone->selectedStones.size();

    if (stoneAmount == 0)
    {
        battleMenu->setVisible(false);
        canSelectStone = true;
    }
    else if (stoneAmount >= 1 && stoneAmount < 4)
    {
        battleMenu->setVisible(true);
        canSelectStone = true;
    }
    else if (stoneAmount == 4)
    {
        battleMenu->setVisible(true);
        canSelectStone = false;
    }
    else // 4 이상
    {
        battleMenu->setVisible(false);
        canSelectStone = false;
    }
}

void HelloWorld::StartBattle(Ref* pSender)
{
    battleState = BattleState::BATTLE;

    auto hide = CallFunc::create([=]()->void
        {
            playerStone->HideAll();
            enemyStone ->HideAll();
        });

    auto showCurStone = CallFunc::create([=]()->void
        {
            enemyStone->PushRandomStones(playerStone->selectedStones.size());

            playerStone->ShowCurrentStone();
            enemyStone ->ShowCurrentStone();

        });

    auto compareStone = CallFunc::create([=]()->void
        {
            CompareStone(playerStone->GetCurrentStone(), enemyStone->GetCurrentStone());
        });

    auto hideCurStone = CallFunc::create([=]()->void
        {
            playerStone->HideCurrentStone();
            enemyStone ->HideCurrentStone();
        });

    auto show = CallFunc::create([=]()->void
        {
            playerStone->ShowAll();
            enemyStone->ShowAll();
        });

    auto end = CallFunc::create([=]()->void
        {
            playerStone->EndBattle();
            enemyStone->EndBattle();

            canSelectStone = true;

            player->ChangeState(CharState::IDLE);
            enemy->ChangeState(CharState::IDLE);

            battleState = BattleState::SELECT_STONE;
        });

#pragma region 최종

    Sequence* battleSeq;

    switch (playerStone->selectedStones.size())
    {
    case 1:
        battleSeq = Sequence::create(
            hide, DelayTime::create(0.5f),
            showCurStone, DelayTime::create(1.0f),
            compareStone, DelayTime::create(2.0f),
            hideCurStone, DelayTime::create(1.0f),
            show, DelayTime::create(0.5f),
            end, DelayTime::create(0.5f),
            nullptr);
        break;
    case 2:
        battleSeq = Sequence::create(
            hide, DelayTime::create(0.5f),
            showCurStone, DelayTime::create(1.0f),
            compareStone, DelayTime::create(2.0f),
            hideCurStone, DelayTime::create(1.0f),
            showCurStone, DelayTime::create(1.0f),
            compareStone, DelayTime::create(2.0f),
            hideCurStone, DelayTime::create(1.0f),
            show, DelayTime::create(0.5f),
            end, DelayTime::create(0.5f),
            nullptr);
        break;
    case 3:
        battleSeq = Sequence::create(
            hide, DelayTime::create(0.5f),
            showCurStone, DelayTime::create(1.0f),
            compareStone, DelayTime::create(2.0f),
            hideCurStone, DelayTime::create(1.0f),
            showCurStone, DelayTime::create(1.0f),
            compareStone, DelayTime::create(2.0f),
            hideCurStone, DelayTime::create(1.0f),
            showCurStone, DelayTime::create(1.0f),
            compareStone, DelayTime::create(2.0f),
            hideCurStone, DelayTime::create(1.0f),
            show, DelayTime::create(0.5f),
            end, DelayTime::create(0.5f),
            nullptr);
        break;
    case 4:
        battleSeq = Sequence::create(
            hide, DelayTime::create(0.5f),
            showCurStone, DelayTime::create(1.0f),
            compareStone, DelayTime::create(2.0f),
            hideCurStone, DelayTime::create(1.0f),
            showCurStone, DelayTime::create(1.0f),
            compareStone, DelayTime::create(2.0f),
            hideCurStone, DelayTime::create(1.0f),
            showCurStone, DelayTime::create(1.0f),
            compareStone, DelayTime::create(2.0f),
            hideCurStone, DelayTime::create(1.0f),
            showCurStone, DelayTime::create(1.0f),
            compareStone, DelayTime::create(2.0f),
            hideCurStone, DelayTime::create(1.0f),
            show, DelayTime::create(0.5f),
            end, DelayTime::create(0.5f),
            nullptr);
        break;
    }

#pragma endregion

    this->runAction(battleSeq);    
}

void HelloWorld::CompareStone(Stone* playerStone, Stone* enemyStone)
{
    int intervalBtwPower = playerStone->powerValue - enemyStone->powerValue;

    if (intervalBtwPower > 0)
    {
        player  ->Attack(this, playerStone);
        enemy   ->SufferDamage(intervalBtwPower);
        return;
    }
    else if (intervalBtwPower < 0)
    {
        enemy   ->Attack(this, enemyStone);
        player  ->SufferDamage(intervalBtwPower * (-1));
        return;
    }
    else
    {
        //부딫혀서 충돌하는 애니메이션 있으면 좋을것 같다 ㅎㅎ
        return;
    }
}

Size HelloWorld::GetScreenSize()
{
    return Director::getInstance()->getVisibleSize();
}

Vec2 HelloWorld::GetScreenMiddlePos()
{
    auto screenSize = Director::getInstance()->getVisibleSize(); 

    return Vec2(screenSize.width / 2, screenSize.height / 2);
}


