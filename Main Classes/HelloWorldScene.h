#ifndef __HELLOWORLD_SCENE_H__
#define __HELLOWORLD_SCENE_H__

#pragma once

#include "cocos2d.h"
#include "Custom Classes/Character/Character.h"
#include "Custom Classes/Enum Collection/EnumCollection.h"
#include "Custom Classes/Status/StatusPanel.h"
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

    Sprite*         bgSprite;
    bool            canSelect;

    //-----[커스텀 객체]

    StatusPanel*    playerStatus;
    StatusPanel*    enemyStatus;

    StonePanel*     playerStone;
    StonePanel*     enemyStone;

    Character*      player;
    Character*      enemy;

    //-----[이벤트]

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

    //-----[게임 진행 관련]

    void StartBattle(Ref* pSender);
    void CompareStone(Stone* playerStone, Stone* enemyStone);
};

#endif
