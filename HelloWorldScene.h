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



#ifndef __HELLOWORLD_SCENE_H__
#define __HELLOWORLD_SCENE_H__

#include "cocos2d.h"
#include "proj.win32/StonePanel.h"

enum GameMode { Choose, Battle };

class HelloWorld : public cocos2d::Scene
{
public:
    static cocos2d::Scene* createScene();    
    virtual bool init();    
    CREATE_FUNC(HelloWorld);

    StonePanel* playerPanel;
    StonePanel* enemyPanel;

    cocos2d::Sprite* player;
    cocos2d::Sprite* enemy;

    cocos2d::EventListenerTouchOneByOne* listener;

    void InitForBattle();

    void onEnter();
    void onExit();
    bool onTouchBegan(cocos2d::Touch* touch, cocos2d::Event* event);
    void onTouchEnded(cocos2d::Touch* touch, cocos2d::Event* event);

    void update(float delta);

    void doBattle(int battlePhase);
    void doNext();
    void doChoose();

    GameMode mode = GameMode::Choose;

    int width, height;

    int battlePhase = 0;

    Stone* getCurrentStone(StonePanel* panel, int value);

    void CompareStone(Stone* player, Stone* enemy);

    void ShowStone(Stone* stone, bool value);

    void TurnToChoose();

    // 터치 인식과 관련된 함수
    void CheckStonesTouch(cocos2d::Vec2 touchPoint);
    void CheckBattleButtonTouch(cocos2d::Vec2 touchPoint);

    void Attack(cocos2d::Sprite* character, float time);

};

#endif // __HELLOWORLD_SCENE_H__
