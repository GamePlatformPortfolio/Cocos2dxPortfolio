#include "StonePanel.h"

Stone::Stone()
{

}

bool Stone::getIsSelected()
{
	return isSelected;
}

void Stone::setIsSelected(bool value)
{
	if (value)
	{
		// Change Selected Option Color
		sprite->setColor(cocos2d::Color3B::YELLOW);
	}
	else
	{
		// Change Color using StoneType
		setStoneColor(type);
	}
	isSelected = value;
}

StoneType Stone::getType()
{
	return type;
}

int Stone::getPower()
{
	return power;
}

void Stone::setStoneColor(StoneType value)
{
	type = value;

	switch (type)
	{
	case StoneType::Null :
		sprite->setColor(cocos2d::Color3B::GREEN);
		break;
	case StoneType::Attack:
		sprite->setColor(cocos2d::Color3B::RED);
		break;
	case StoneType::Guard:
		sprite->setColor(cocos2d::Color3B::BLUE);
		break;
	}
}

void Stone::Init(std::string fileName, cocos2d::Rect rect, cocos2d::Vec2 position)
{
	sprite = cocos2d::Sprite::create(fileName, rect);
	sprite->setPosition(position);

	power = rand() % 5;
	powerLabal = cocos2d::Label::createWithSystemFont(std::to_string(power), "", 30);
	powerLabal->setColor(cocos2d::Color3B::BLACK);
	powerLabal->setPosition(cocos2d::Vec2(sprite->getContentSize().width / 2, sprite->getContentSize().height / 2));
	sprite->addChild(powerLabal);	
}

// StonePanel 

bool StonePanel::getBattleButton()
{
	return showBattleButton;
}

void StonePanel::setBattleButton(bool value)
{
	if (value)
	{
		// show BattleButton
	}
	showBattleButton = value;
}

StonePanel::StonePanel(cocos2d::Scene* drawScene, std::string fileName, cocos2d::Rect rect, 
	cocos2d::Vec2 position, cocos2d::Color3B color)
{
	sprite = cocos2d::Sprite::create(fileName, rect);
	sprite->setPosition(position);
	sprite->setColor(color);
	drawScene->addChild(sprite);

	cocos2d::Rect stoneRect = cocos2d::Rect(0, 0, 40, 40);

	// Init Stones
	stones = new Stone[6];
	srand((unsigned int)time(0));
	for (int i = 0; i < 6; i++)
	{
		stones[i].Init(fileName, stoneRect, 
			cocos2d::Vec2((i + 1) * sprite->getContentSize().width / 7, sprite->getPosition().y));

		stones[i].setStoneColor(getRandomType());
		drawScene->addChild(stones[i].sprite);
	}

	// Init BattleButton
	battleButton = cocos2d::Sprite::create(fileName, cocos2d::Rect(0, 0, 160, 80));
	battleButton->setPosition(cocos2d::Vec2(360, 240));
	battleButton->setColor(cocos2d::Color3B::GREEN);

	drawScene->addChild(battleButton);

	battleButton->setVisible(false);
}

StoneType StonePanel::getRandomType()
{
	int i = rand() % 3;
	return (StoneType)i;
}

void StonePanel::SelectStone(Stone* stone, bool value)
{
	if (value)
	{
		if (showBattleButton)
			return;

		setSelectedCount(1);
		selectedStones.push_back(stone);
	}
	else if (!value)
	{
		// Remove from List
		setSelectedCount(-1);
		selectedStones.remove(stone);
	}
	stone->setIsSelected(value);
}

void StonePanel::setSelectedCount(int value)
{
	selectedCount += value;
	//cocos2d::log("%i", selectedCount);
	if (selectedCount >= 3)
	{
		showBattleButton = true;
		battleButton->setVisible(true);
	}
	else
	{
		showBattleButton = false;
		battleButton->setVisible(false);
	}
}