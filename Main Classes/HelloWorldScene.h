#ifndef __HELLOWORLD_SCENE_H__
#define __HELLOWORLD_SCENE_H__

#pragma once

#include "cocos2d.h"
#include "Custom Classes/Character/Character.h"
#include "Custom Classes/Enum Collection/EnumCollection.h"
#include "Custom Classes/StatPanel/StatPanel.h"
#include "Custom Classes/Character/EpPanel.h"

#include "Custom Classes/Stone/Stone.h"
#include "Custom Classes/Stone/StonePanel.h"

using namespace cocos2d;
using namespace std;

class HelloWorld : public cocos2d::Scene
{
public:
    static cocos2d::Scene* createScene();

    virtual bool init();    

    CREATE_FUNC(HelloWorld);

    Sprite* bgSprite;
    bool canSelect;

    bool isHold;

    const string spriteRootFolder = "Images/";
    const string audioRootFolder = "Sounds/";

    StonePanel* playerStone;
    StonePanel* enemyStone;

    Character* player;
    Character* enemy;

    StatPanel* playerStat;
    StatPanel* enemyStat;

    EpPanel* epDisplay;

    // About Touch Event 
    Menu* pMenu;
    MenuItemFont* pMenuItem1;
    EventListenerTouchOneByOne* listener;

    void onEnter();
    void onExit();
    bool onTouchBegan(Touch* touch, Event* event);
    void onTouchMoved(Touch* touch, Event* event);
    void onTouchEnded(Touch* touch, Event* event);
    void onTouchCancelled(Touch* touch, Event* event);

    void update(float dt);

    // About Game Battle
    void StartBattle(Ref* pSender);
    void CompareStone(Stone* playerStone, Stone* enemyStone);

    // ETC
    Size GetScreenSize();
    Vec2 GetScreenMiddlePos();
};

#endif
