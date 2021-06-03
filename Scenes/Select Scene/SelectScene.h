#ifndef __SELECTWORLD_SCENE_H__
#define __SELECTWORLD_SCENE_H__

#pragma once

#include "cocos2d.h"
#include "HelloWorldScene.h"
#include "AudioEngine.h"

using namespace cocos2d;

class SelectScene : public Scene
{
public:
    static Scene* createScene();

    virtual bool init();

    CREATE_FUNC(SelectScene);

    void onEnter();
    void onExit();
    bool onTouchBegan(Touch* touch, Event* event);
    void onTouchMoved(Touch* touch, Event* event);
    void onTouchEnded(Touch* touch, Event* event);
    void onTouchCancelled(Touch* touch, Event* event);

    void SelectStageCallback(Ref* pSender);
    void exitSelectSceneCallback(Ref* pSender);

private:
    EventListenerTouchOneByOne* listener;

    Sprite* bgImg;
    Sprite* markerImg;
    int bgmId;

    Rect stageSelectRect;
    Vec2 drawMakerPos;
};
#endif

