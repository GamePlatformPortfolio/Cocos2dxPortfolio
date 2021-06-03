#ifndef __HELLOWORLD_SCENE_H__
#define __HELLOWORLD_SCENE_H__

#pragma once

#include "cocos2d.h"
#include "Custom Classes/Character/Character.h"
#include "Custom Classes/Enum Collection/EnumCollection.h"
#include "Custom Classes/Stone/Stone.h"
#include "Custom Classes/Stone/StonePanel.h"

using namespace cocos2d;
using namespace std;

#define SPRITE_ROOT_FOLDER  "Images/"
#define AUDIO_ROOT_FOLDER   "Sounds/"

typedef enum BattleState { SELECT_STONE, BATTLE };

class HelloWorld : public cocos2d::Scene
{
#pragma region 변수
public:
    Sprite* bgSprite;
    bool canSelectStone;

    bool isMouseHolding;

    StonePanel* playerStone;
    StonePanel* enemyStone;

    Character* player;
    Character* enemy;

    Menu* battleMenu;
    MenuItemFont* battleMenuItem;
    EventListenerTouchOneByOne* listener;

    BattleState battleState = BattleState::SELECT_STONE;
#pragma endregion

#pragma region 함수
public:
    static cocos2d::Scene* createScene();

    virtual bool init();

    CREATE_FUNC(HelloWorld);

    // 초기화 함수
    void InitStonePanel();
    void InitCharacter();
    void InitCocosContent(); // 배경 이미지, 메뉴 초기화
    // --------------------

    // 터치 이벤트 함수
    void onEnter();
    void onExit();
    bool onTouchBegan(Touch* touch, Event* event);
    void onTouchMoved(Touch* touch, Event* event);
    void onTouchEnded(Touch* touch, Event* event);
    void onTouchCancelled(Touch* touch, Event* event);
    // --------------------

    void update(float dt);

    // 배틀 이벤트 함수
    void StartBattle(Ref* pSender);
    void CompareStone(Stone* playerStone, Stone* enemyStone);
    // --------------------

    // 스크린 크기, 위치 관련 함수
    Size GetScreenSize();
    Vec2 GetScreenMiddlePos();
    // --------------------
#pragma endregion
};

#endif
