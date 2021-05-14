#include "HelloWorldScene.h"
#include "SimpleAudioEngine.h"
#include "AudioEngine.h"
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

    scheduleUpdate();
    AudioEngine::play2d("BGM.mp3", true, 0.1f);
    auto screenSize = Director::getInstance()->getVisibleSize();

    canSelect = true;

    string bgFileName = "Background.png";
    bgSprite = Sprite::create(bgFileName);//"Images/" + 
    bgSprite->setContentSize(screenSize);
    bgSprite->setPosition(screenSize / 2);
    this->addChild(bgSprite);


    pMenuItem1 = MenuItemFont::create("Battle", CC_CALLBACK_1(HelloWorld::StartBattle, this));
    pMenuItem1->setColor(Color3B::BLACK);

    pMenu = Menu::create(pMenuItem1, nullptr);
    pMenu->alignItemsHorizontally();
    this->addChild(pMenu); 

    //-----[커스텀 객체 생성 및 초기화]

    playerStone = new StonePanel(CharacterType::Player,
        "StonePanel.png",
        Vec2(screenSize.width / 2, screenSize.height / 8),
        Size(screenSize.width, screenSize.height / 4));

    enemyStone = new StonePanel(CharacterType::Enemy,
        "StonePanel.png",
        Vec2(screenSize.width / 2, screenSize.height - screenSize.height / 8),
        Size(screenSize.width, screenSize.height / 4));

    this->addChild(playerStone->panelSprite);

    this->addChild(enemyStone->panelSprite);

    player = new Character(CharacterType::Player,
        "Player.png",
        Vec2(screenSize.width / 4, screenSize.height / 2),
        Size(300, 300));

    enemy = new Character(CharacterType::Enemy,
        "Enemy.png",
        Vec2(screenSize.width / 2 + screenSize.width / 4, screenSize.height / 2),
        Size(300, 300));

    this->addChild(player->GetSprite());
    this->addChild(enemy ->GetSprite());

    playerStatus = new StatusPanel("StatusBG.png", "StatusHP.png", player, Size(30, 100),
        Vec2(50, 0), 100);

    return true;
}

void HelloWorld::onEnter()
{
    Scene::onEnter();

    listener = EventListenerTouchOneByOne::create();

    listener->setSwallowTouches(true);

    listener->onTouchBegan = CC_CALLBACK_2(HelloWorld::onTouchBegan, this);
    listener->onTouchMoved = CC_CALLBACK_2(HelloWorld::onTouchMoved, this);
    listener->onTouchEnded = CC_CALLBACK_2(HelloWorld::onTouchEnded, this);
    listener->onTouchCancelled = CC_CALLBACK_2(HelloWorld::onTouchCancelled, this);

    _eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);
}

void HelloWorld::onExit()
{
    _eventDispatcher->removeEventListener(listener);

    Scene::onExit();
}

bool HelloWorld::onTouchBegan(Touch* touch, Event* event)
{
    auto touchPoint = touch->getLocation();

    bool bTouch = false;
        
    for (int i = 0; i < maxStoneCount; i++)
    {
        bTouch = playerStone->allStones[i]->sprite->getBoundingBox().containsPoint(touchPoint);
        if (bTouch)
        {
            if (playerStone->allStones[i]->GetSelect())
            {
                playerStone->UnSelectedStone(i);
            }
            else if (playerStone->allStones[i]->GetSelect() == false && canSelect)
            {
                playerStone->SelectStone(i);
            }
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
}

void HelloWorld::onTouchCancelled(Touch* touch, Event* event)
{
    auto touchPoint = touch->getLocation();
}

void HelloWorld::update(float dt)
{
    if (playerStone->selectedStones.size() >= 3)
    {
        canSelect = false;
        pMenu->setVisible(true);
    }
    else
    {
        canSelect = true;
        pMenu->setVisible(false);
    }

    if (playerStatus->target != nullptr )
    {
        playerStatus->UpdateHpBar();

    }
}

void HelloWorld::StartBattle(Ref* pSender)
{
    auto hide = CallFunc::create([=]()->void
        {
            playerStone->HideAll();
            enemyStone ->HideAll();
        });

    auto showCurStone = CallFunc::create([=]()->void
        {
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
            enemyStone ->EndBattle();

            canSelect = true;
        });

    auto battleSeq = Sequence::create(
        hide, DelayTime::create(0.5f),
        showCurStone, DelayTime::create(1.0f),
        compareStone, DelayTime::create(2.0f),
        hideCurStone, DelayTime::create(1.0f),
        showCurStone, DelayTime::create(1.0f),
        compareStone, DelayTime::create(1.0f),
        hideCurStone, DelayTime::create(1.0f),
        showCurStone, DelayTime::create(1.0f),
        compareStone, DelayTime::create(2.0f),
        hideCurStone, DelayTime::create(1.0f),
        show, DelayTime::create(0.5f),
        end, DelayTime::create(0.5f),
        nullptr);

    this->runAction(battleSeq);
    
}

void HelloWorld::CompareStone(Stone* playerStone, Stone* enemyStone)
{
    int calculatedDamage = playerStone->GetPower() - enemyStone->GetPower();

    if (calculatedDamage > 0)
    {
        player->Attack();
        AudioEngine::play2d("Craver_MA.mp3", false, 0.1f);
        enemy ->Damaged(calculatedDamage);
        return;
    }
    else if (calculatedDamage < 0)
    {
        enemy ->Attack();
        player->Damaged(-calculatedDamage);
        return;
    }
    else
    {
        return;
    }
}


