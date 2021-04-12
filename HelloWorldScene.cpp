/****************************************************************************
 Copyright (c) 2017-2018 Xiamen Yaji Software Co., Ltd.
 
 http://www.cocos2d-x.org
 
 Permission is hereby granted, free of charge, to any person obtaining a copy
 of this software and associated documentation files (the "Software"), to deal
 in the Software without restriction, including without limitation the rights
 to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 copies of the Software, and to permit persons to whom the Software is
 furnished to do so, subject to the following conditions:
 
 The above copyright notice and this permission notice shall be included in
 all copies or substantial portions of the Software.
 
 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 THE SOFTWARE.
 ****************************************************************************/

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

    int screenWidth = Director::getInstance()->getWinSize().width;
    int screenHeight = Director::getInstance()->getWinSize().height;

    // Background Layer
    auto wlayer = LayerColor::create(Color4B::WHITE);
    this->addChild(wlayer);    

    stonePanel = new StonePanel(this, "Images/white-512x512.png", cocos2d::Rect(0, 0, screenWidth, 120),
        cocos2d::Vec2(screenWidth/2, 60), cocos2d::Color3B::GRAY);

    enemyStonePanel = new StonePanel(this, "Images/white-512x512.png", cocos2d::Rect(0, 0, screenWidth, 120),
        cocos2d::Vec2(screenWidth / 2, screenHeight - 60), cocos2d::Color3B::GRAY);

    scheduleUpdate();

    return true;
}

void HelloWorld::update(float delta)
{

}

void HelloWorld::onEnter()
{
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

//위젯 터치 이벤트 재정의 함수

bool HelloWorld::onTouchBegan(Touch* touch, Event* unusedEvent)
{
    Vec2 LocationInNode = this->convertToNodeSpace(touch->getLocation());

    Vec2 stonePos;
    Size stoneSize;
    Rect stoneRect;

    for (int i = 0; i < 6; i++)
    {
        stonePos = stonePanel->stones[i].sprite->getPosition();
        stoneSize = stonePanel->stones[i].sprite->getContentSize();
        stoneRect = cocos2d::Rect(stonePos.x - stoneSize.width/2, stonePos.y - stoneSize.height/2, stoneSize.width, stoneSize.height);

        if (stoneRect.containsPoint(LocationInNode))
        {
            switch (stonePanel->stones[i].getIsSelected())
            {
            case true:
                stonePanel->SelectStone(&(stonePanel->stones[i]), false);
                break;
            case false:
                stonePanel->SelectStone(&(stonePanel->stones[i]), true);
                break;
            }
            return true;
        }

        Vec2 buttonPos;
        Size buttonSize;
        Rect buttonRect;

        buttonPos = stonePanel->battleButton->getPosition();
        buttonSize = stonePanel->battleButton->getContentSize();
        buttonRect = cocos2d::Rect(buttonPos.x - buttonSize.width / 2, buttonPos.y - buttonSize.height / 2,
            buttonSize.width, buttonSize.height);

        if (stonePanel->battleButton->isVisible() && buttonRect.containsPoint(LocationInNode))
        {
            cocos2d::log("Battle Mode!");
        }

        // 배틀 버튼이 활성화되어있고, 터치했으면 배틀모드 진입!
    }    
    return false;
}

void HelloWorld::onTouchEnded(Touch* touch, Event* unusedEvent)
{
    auto touchPoint = touch->getLocation();  
}

