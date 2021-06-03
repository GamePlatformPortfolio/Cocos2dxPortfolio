#include "SelectScene.h"

Scene* SelectScene::createScene()
{
	return SelectScene::create();
}

bool SelectScene::init()
{
	if (!Scene::init())
		return false;

    stageSelectRect = Rect(230, 230, 100, 100);

	std::string imagePath = "Images/StartScene/";
	std::string soundPath = "Sounds/";

    bgmId = AudioEngine::play2d(soundPath + "BGM_Middle.mp3", true, 0.0f);

    bgImg = Sprite::create(imagePath + "Background.png");
    bgImg->setPosition(
        Director::getInstance()->getWinSize().width / 2,
        Director::getInstance()->getWinSize().height / 2);
    bgImg->setContentSize(Size(
        Director::getInstance()->getWinSize().width,
        Director::getInstance()->getWinSize().height));
    this->addChild(bgImg);

    markerImg = Sprite::create(imagePath + "Marker.png");
    markerImg->setPosition(Vec2::ZERO);
    markerImg->setContentSize(Size(100, 100));
    markerImg->setVisible(false);
    this->addChild(markerImg);

	return true;
}

void SelectScene::onEnter()
{
    Scene::onEnter();

    listener = EventListenerTouchOneByOne::create();

    listener->setSwallowTouches(true);

    listener->onTouchBegan = CC_CALLBACK_2(SelectScene::onTouchBegan, this);
    listener->onTouchMoved = CC_CALLBACK_2(SelectScene::onTouchMoved, this);
    listener->onTouchEnded = CC_CALLBACK_2(SelectScene::onTouchEnded, this);
    listener->onTouchCancelled = CC_CALLBACK_2(SelectScene::onTouchCancelled, this);

    _eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);
}

void SelectScene::onExit()
{
    _eventDispatcher->removeEventListener(listener);

    Scene::onExit();
}

bool SelectScene::onTouchBegan(Touch* touch, Event* event)
{
    auto touchPoint = touch->getLocation();

    bool isSelected = stageSelectRect.containsPoint(touchPoint);
    if (isSelected)
    {
        markerImg->setPosition(drawMakerPos);
        markerImg->setVisible(true);

        SelectStageCallback(this);
    }
}

void SelectScene::onTouchMoved(Touch* touch, Event* event)
{
}

void SelectScene::onTouchEnded(Touch* touch, Event* event)
{
}

void SelectScene::onTouchCancelled(Touch* touch, Event* event)
{
}

void SelectScene::SelectStageCallback(Ref* pSender)
{
    auto scaleUp = ScaleTo::create(1.0f, 1.5f);
    auto scaleOrigin = ScaleTo::create(1.0f, 1.0f);

    auto scaleSeq = Sequence::create(scaleUp, scaleOrigin, NULL);

    markerImg->runAction(scaleSeq);
}
