#include "StartScene.h"
#include "SimpleAudioEngine.h"

Scene* StartScene::createScene()
{
	return StartScene::create();
}

bool StartScene::init()
{
    if (!Scene::init())    
        return false;

    soundFadeTime = 5.0f;

    std::string imagePath = "Images/StartScene/";
    std::string soundPath = "Sounds/";

    bgmId = AudioEngine::play2d(soundPath + "BGM.mp3", true, 0.0f);

    bgImg = Sprite::create(imagePath + "Background.png");
    bgImg->setPosition(
        Director::getInstance()->getWinSize().width / 2,
        Director::getInstance()->getWinSize().height / 2);
    bgImg->setContentSize(Size(
        Director::getInstance()->getWinSize().width,
        Director::getInstance()->getWinSize().height));
    this->addChild(bgImg);

    auto item_1 = MenuItemImage::create(
        imagePath + "Btn_PushStart.png",
        imagePath + "Btn_Start.png",
        imagePath + "Btn_Start.png",
        CC_CALLBACK_1(StartScene::startGameCallback, this));

    auto item_2 = MenuItemImage::create(
        imagePath + "Btn_PushExit.png",
        imagePath + "Btn_Exit.png",
        imagePath + "Btn_Exit.png",
        CC_CALLBACK_1(StartScene::exitGameCallback, this));

    auto menu = Menu::create(item_1, item_2, NULL);

    menu->alignItemsVertically();
    menu->setPositionY(Director::getInstance()->getWinSize().height / 4);
    this->addChild(menu);

    scheduleUpdate();
    
    return true;
}

void StartScene::startGameCallback(Ref* pSender)
{
    Director::getInstance()->replaceScene(TransitionFade::create(1, HelloWorld::createScene()));
}

void StartScene::exitGameCallback(Ref* pSender)
{
    Director::getInstance()->end();
}

void StartScene::update(float delta)
{
    if (!soundFadeIsEnd)
    {
        if (currentVolume > 1.0f)
        {
            currentVolume = maxVolume;
            soundFadeIsEnd = true;
        }

        currentVolume += delta / soundFadeTime;

        AudioEngine::setVolume(bgmId, currentVolume);
    }
}
