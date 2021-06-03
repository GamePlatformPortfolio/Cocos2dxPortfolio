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
#pragma region Super_Init
    if ( !Scene::init() )
    {
        return false;
    }

    srand((unsigned int)time(NULL));
#pragma endregion
    canSelect = true;
#pragma region Init_BackGround
    // Init Background
    string bgFileName = "Background.png";
    bgSprite = Sprite::create(spriteRootFolder + bgFileName);
    bgSprite->setPosition(GetScreenMiddlePos());
    bgSprite->setContentSize(GetScreenSize());
    this->addChild(bgSprite);

    AudioEngine::play2d("Sounds/BGM.mp3", true, 0.1f);
#pragma endregion
#pragma region Init_Menu
    // Init Menu
    pMenuItem1 = MenuItemFont::create("Battle", CC_CALLBACK_1(HelloWorld::StartBattle, this));
    pMenuItem1->setColor(Color3B::BLACK);

    pMenu = Menu::create(pMenuItem1, nullptr);
    pMenu->alignItemsHorizontally();
    this->addChild(pMenu); 
#pragma endregion
#pragma region Init_StonePanel
    // Init Stone Panel
    playerStone = new StonePanel(
        CharType::PLAYER,
        spriteRootFolder,
        Vec2(GetScreenSize().width / 2, GetScreenSize().height / 8),
        Size(GetScreenSize().width, GetScreenSize().height / 4)
    );
    this->addChild(playerStone->panelSprite);

    enemyStone = new StonePanel(
        CharType::ENEMY,
        spriteRootFolder,
        Vec2(GetScreenSize().width / 2, GetScreenSize().height - GetScreenSize().height / 8),
        Size(GetScreenSize().width, GetScreenSize().height / 4)
    );
    this->addChild(enemyStone->panelSprite);
#pragma endregion
#pragma region Init_Character
    // Init Character
    player = new Character(
        CharType::PLAYER,
        spriteRootFolder,
        Vec2(GetScreenMiddlePos() - Vec2(GetScreenSize().width / 4, 0)),
        Size(300, 300)
    );
    this->addChild(player->GetSprite());

    enemy = new Character(
        CharType::ENEMY,
        spriteRootFolder,
        Vec2(GetScreenMiddlePos() + Vec2(GetScreenSize().width / 4, 0)),
        Size(300, 300)
    );
    this->addChild(enemy ->GetSprite());
#pragma endregion
#pragma region Init_EpPanel
    epDisplay = new EpPanel(playerStone, player);
    this->addChild(epDisplay->GetSprite());
#pragma endregion

    scheduleUpdate();

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
        
    for (int i = 0; i < handAmount; i++){
        bool bTouch = playerStone->handStones[i]->GetSprite()->getBoundingBox().containsPoint(touchPoint);
        if (bTouch){
            if (playerStone->handStones[i]->GetSelect()){
                playerStone->UnSelectedStone(i);
            }
            else if (playerStone->handStones[i]->GetSelect() == false && canSelect){
                if (epDisplay->GetPredEP() + playerStone->handStones[i]->GetEpUsage() > player->GetCurrentEp()) return true;
                playerStone->SelectStone(i);
            }
            playerStone->handStones[i]->ShowInform();
            epDisplay->Update(UpdateType::Selection);
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

    for (int i = 0; i < handAmount; i++) { 
    playerStone->handStones[i]->HideInform(); 
    }
}

void HelloWorld::onTouchCancelled(Touch* touch, Event* event){
    auto touchPoint = touch->getLocation();
}

void HelloWorld::update(float dt){
    if (playerStone->selectedStones.size() >= 4) { canSelect = false; }
    else                                         { canSelect = true; }

    if (playerStone->selectedStones.size() > 0) pMenu->setVisible(true);
    else                                        pMenu->setVisible(false);
}

void HelloWorld::StartBattle(Ref* pSender)
{
#pragma region Action List
    auto hide = CallFunc::create([=]()->void
        {
            playerStone->HideAll();
            enemyStone->HideAll();
            epDisplay->HideAll();
        });
    auto showCurStone = CallFunc::create([=]()->void
        {
            enemyStone->PushRandomStones(playerStone->selectedStones.size());
            playerStone->ShowCurrentStone();
            enemyStone->ShowCurrentStone();

        });
    auto compareStone = CallFunc::create([=]()->void
        {
            CompareStone(playerStone->GetCurrentStone(), enemyStone->GetCurrentStone());
        });
    auto hideCurStone = CallFunc::create([=]()->void
        {
            playerStone->HideCurrentStone();
            enemyStone->HideCurrentStone();
        });
    auto show = CallFunc::create([=]()->void
        {
            playerStone->ShowAll();
            enemyStone->ShowAll();
            epDisplay->ShowAll();
        });
    auto end = CallFunc::create([=]()->void
        {
            playerStone->EndBattle();
            enemyStone->EndBattle();

            canSelect = true;
        });
    auto clearSelected = CallFunc::create([=]()->void{
    playerStone->ClearSelectedStone();
    enemyStone->ClearSelectedStone();
    });
    auto updateEpPanel = CallFunc::create([=]()->void{
    epDisplay->Update(UpdateType::Selection);
    });
    auto regenEp = CallFunc::create([=]()->void {
        player->RegenEp();
        enemy->RegenEp();
        });
#pragma endregion
#pragma region Final Sequence

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
            regenEp, updateEpPanel,
            nullptr);
        break;
    case 2:
        battleSeq = Sequence::create(
            updateEpPanel,
            hide, DelayTime::create(0.5f),
            showCurStone, DelayTime::create(1.0f),
            compareStone, DelayTime::create(2.0f),
            hideCurStone, DelayTime::create(1.0f),
            showCurStone, DelayTime::create(1.0f),
            compareStone, DelayTime::create(2.0f),
            hideCurStone, DelayTime::create(1.0f),
            show, DelayTime::create(0.5f),
            end, DelayTime::create(0.5f),
            regenEp, updateEpPanel,
            nullptr);
        break;
    case 3:
        battleSeq = Sequence::create(
            updateEpPanel,
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
            regenEp, updateEpPanel,
            nullptr);
        break;
    case 4:
        battleSeq = Sequence::create(
            updateEpPanel,
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
            regenEp, updateEpPanel,
            nullptr);
        break;
    }

#pragma endregion
    player->UseEp(epDisplay->GetPredEP());
    epDisplay->Update(UpdateType::Result);
    this->runAction(battleSeq);    
}

void HelloWorld::CompareStone(Stone* playerStone, Stone* enemyStone)
{
    int damageValue = playerStone->GetPower() - enemyStone->GetPower();
    if (damageValue > 0) {
        player->Action(playerStone);
        enemy->SufferDamage(damageValue);
        return;
    }
    else if (damageValue < 0) {
        enemy->Action(enemyStone);
        player->SufferDamage(damageValue * (-1));
        return;
    }
    else {
        //부딫혀서 충돌하는 애니메이션 있으면 좋을것 같다 ㅎㅎ
        return;
    }
    //if (damageValue > 0) {

    //    enemy->SufferDamage(damageValue);
    //}
    //else if (damageValue < 0) {

    //    player->SufferDamage(damageValue);
    //}
    //else {
    //    
    //    //Conflict Animation
    //}


}

Size HelloWorld::GetScreenSize()
{
    Size screenSize = Director::getInstance()->getVisibleSize();

    return screenSize;
}

Vec2 HelloWorld::GetScreenMiddlePos()
{
    Size screenSize = Director::getInstance()->getVisibleSize();

    Vec2 middlePos = Vec2(screenSize.width / 2, screenSize.height / 2);

    return middlePos;
}


