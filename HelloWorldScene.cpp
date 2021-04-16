#include "HelloWorldScene.h"
#include "SimpleAudioEngine.h"
#include "AppDelegate.h"

#include "cocos2d.h"


USING_NS_CC;

Scene* HelloWorld::createScene()
{
    return HelloWorld::create();
}

// Print useful error message instead of segfaulting when files are not there.
static void problemLoading(const char* filename)
{
    printf("Error while loading: %s\n", filename);
    printf("Depending on how you compiled you might have to add 'Resources/' in front of filenames in HelloWorldScene.cpp\n");
}

// on "init" you need to initialize your instance
bool HelloWorld::init()
{
    if (!Scene::init())
    {
        return false;
    }

    width = Director::getInstance()->getWinSize().width;
    height = Director::getInstance()->getWinSize().height;

    // 배경 레이어
    auto wlayer = LayerColor::create(Color4B::WHITE);
    this->addChild(wlayer);    

    // 플레이어
    player = Sprite::create("Images/white-512x512.png", Rect(0, 0, 100, 200));
    player->setPosition(width / 2 - width / 4, height / 2);
    player->setColor(Color3B::ORANGE);
    this->addChild(player);

    // 적
    enemy = Sprite::create("Images/white-512x512.png", Rect(0, 0, 100, 200));
    enemy->setPosition(width /2 + width / 4, height / 2);
    enemy->setColor(Color3B::ORANGE);
    this->addChild(enemy);

    // 각 캐릭터의 스톤 패널
    playerPanel = new StonePanel(this, "Images/white-512x512.png", Rect(0, 0, width, 120),
        cocos2d::Vec2(width /2, 60), Color3B::GRAY);

    enemyPanel = new StonePanel(this, "Images/white-512x512.png", Rect(0, 0, width, 120),
        cocos2d::Vec2(width / 2, height - 60), Color3B::GRAY);

    // 업데이트
    scheduleUpdate();

    return true;
}

void HelloWorld::update(float delta)
{

}

void HelloWorld::ShowStone(Stone* stone, bool value)
{
    stone->sprite->setVisible(value);
    stone->powerLabal->setVisible(value);
}

void HelloWorld::onEnter()
{
    // 이벤트 리스너 등록
    Scene::onEnter();

    listener = EventListenerTouchOneByOne::create();
    listener->setSwallowTouches(true);
    listener->onTouchBegan = CC_CALLBACK_2(HelloWorld::onTouchBegan, this);
    listener->onTouchEnded = CC_CALLBACK_2(HelloWorld::onTouchEnded, this);

    _eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);
}

void HelloWorld::onExit()
{
    _eventDispatcher->removeEventListener(listener);

    Scene::onExit();
}

bool HelloWorld::onTouchBegan(Touch* touch, Event* unusedEvent)
{
    Vec2 LocationInNode = this->convertToNodeSpace(touch->getLocation());

    CheckStonesTouch(LocationInNode);

    CheckBattleButtonTouch(LocationInNode);

    return false;
}

void HelloWorld::CheckStonesTouch(Vec2 touchPoint)
{
    Vec2 stonePos;
    Size stoneSize;
    Rect stoneRect;

    for (int i = 0; i < 6; i++)
    {
        stonePos  = playerPanel->stones[i].sprite->getPosition();
        stoneSize = playerPanel->stones[i].sprite->getContentSize();
        stoneRect = Rect(stonePos.x - stoneSize.width / 2, stonePos.y - stoneSize.height / 2, stoneSize.width, stoneSize.height);

        if (stoneRect.containsPoint(touchPoint)) // 스톤패널에 있는 스톤범위에 클릭한 마우스 좌표가 포함될 경우
        {
            switch (playerPanel->stones[i].getIsSelected())
            {
            case true:
                playerPanel->SelectStone(&(playerPanel->stones[i]), false);
                break;
            case false:
                playerPanel->SelectStone(&(playerPanel->stones[i]), true);
                break;
            }
        }
    }
}

void HelloWorld::CheckBattleButtonTouch(Vec2 touchPoint)
{
    Vec2 buttonPos;
    Size buttonSize;
    Rect buttonRect;

    buttonPos  = playerPanel->battleButton->getPosition();
    buttonSize = playerPanel->battleButton->getContentSize();
    buttonRect = cocos2d::Rect(buttonPos.x - buttonSize.width / 2, buttonPos.y - buttonSize.height / 2, buttonSize.width, buttonSize.height);

    // 버튼이 표시되고, 마우스 좌표가 범위에 포함되어 있을 경우
    if (playerPanel->battleButton->isVisible() && buttonRect.containsPoint(touchPoint))
    {
        cocos2d::log("Into Battle Mode!");
        InitForBattle();
    }
}

void HelloWorld::onTouchEnded(Touch* touch, Event* unusedEvent)
{
    auto touchPoint = touch->getLocation();  
}

Stone* HelloWorld::getCurrentStone(StonePanel* panel, int value)
{
    int i = 0;
    for (Stone* stone : panel->selectedStones)
    {
        if (i == value)
            return stone;
        else 
            i++;
    }
}

void HelloWorld::InitForBattle()
{
    for (int i = 0; i < 3; i++)
    {
        enemyPanel->selectedStones.push_back(&enemyPanel->stones[i]);
    }

    playerPanel->battleButton->setVisible(false);

    for (int i = 0; i < 6; i++)
    {
        playerPanel->stones[i].setIsSelected(false);
        playerPanel->stones[i].powerLabal->setVisible(false);

        enemyPanel->stones[i].setIsSelected(false);
        enemyPanel->stones[i].powerLabal->setVisible(false);
    }  

    doBattle(battlePhase);
}

void HelloWorld::doBattle(int phase)
{
    //1. 캐릭터들을 중앙까지 이동시킵니다.

    //2. 캐릭터들의 스톤값을 비교하고 공격을 실행합니다.
    Stone* playerStone = getCurrentStone(playerPanel, phase);
    Stone* enemyStone = getCurrentStone(enemyPanel, phase);


    auto compareStone = CallFunc::create(CC_CALLBACK_0(HelloWorld::CompareStone, this, playerStone, enemyStone));

    auto showPlayerStone = CallFunc::create(
        CC_CALLBACK_0(HelloWorld::ShowStone, this, playerStone, true));

    auto showEnemyStone = CallFunc::create(
        CC_CALLBACK_0(HelloWorld::ShowStone, this, enemyStone, true));

    auto battleSequence = Sequence::create(showPlayerStone, showEnemyStone,
        compareStone, nullptr);

    this->runAction(battleSequence);
}

void HelloWorld::TurnToChoose()
{
    // 리스트 제거, 스프라이트 상태 초기화
    playerPanel->selectedStones.clear();
    enemyPanel->selectedStones.clear();

    playerPanel->setSelectedCount(-3);
    battlePhase = 0;

    for (int i = 0; i < 6; i++)
    {
        playerPanel->stones[i].powerLabal->setVisible(true);
        enemyPanel->stones[i].powerLabal->setVisible(true);
    }
}

void HelloWorld::doNext()
{
    if (battlePhase >= 2)
        TurnToChoose(); // 선택모드로 돌아갑니다.
    else
    {
        doBattle(++battlePhase);
    }
}

void HelloWorld::CompareStone(Stone* playerStone, Stone* enemyStone)
{
    auto playerMoveFront = MoveBy::create(0.25f, Vec2(player->getContentSize().width, 0));
    auto pEaseFront = EaseOut::create(playerMoveFront, 0.25f);
    auto playerMoveBack = MoveBy::create(0.25f, Vec2(-(player->getContentSize().width), 0));
    auto pEaseBack = EaseOut::create(playerMoveBack, 0.25f);

    auto enemyMoveBack = MoveBy::create(0.25f, Vec2(enemy->getContentSize().width, 0));
    auto eEaseBack = EaseOut::create(enemyMoveBack, 0.25f);
    auto enemyMoveFront = MoveBy::create(0.25f, Vec2(-(enemy->getContentSize().width), 0));
    auto eEaseFront = EaseOut::create(enemyMoveFront, 0.25f);

    auto next = CallFunc::create(CC_CALLBACK_0(HelloWorld::doNext, this));

    if (playerStone->getPower() > enemyStone->getPower())         // 플레이어 > 적
    {
        auto temp = Sequence::create(pEaseFront, DelayTime::create(0.25f), pEaseBack, DelayTime::create(0.5f), next, nullptr);
        player->runAction(temp);
    }
    else if (playerStone->getPower() < enemyStone->getPower())    // 플레이어 < 적
    {
        auto temp = Sequence::create(eEaseFront, DelayTime::create(0.25f), eEaseBack, DelayTime::create(0.5f), next, nullptr);
        enemy->runAction(temp);
    }
    else                                                // 플레이어 = 적
    {
        auto temp = Sequence::create(DelayTime::create(1.25f), next, nullptr);
        this->runAction(temp);
    }    

}

void HelloWorld::Attack(Sprite* character, float time)
{

}



