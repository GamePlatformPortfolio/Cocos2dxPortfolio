#pragma once

#include"cocos2d.h"
#include <cstdlib>
#include <ctime>

enum StoneType { Null, Attack, Guard };

class Stone
{
private:
	StoneType type = StoneType::Null;
	bool isSelected = false;
	int power = 0;
public:
	cocos2d::Sprite* sprite;
	cocos2d::Label* powerLabal;

	bool getIsSelected(); 
	void setIsSelected(bool value); // change stone color when stone is selected.

	int getPower(); // readOnly
	StoneType getType(); // readOnly

	Stone();
	void Init(std::string fileName, cocos2d::Rect rect, cocos2d::Vec2 position);
	//~Stone();

	void setStoneColor(StoneType value);
};

class StonePanel
{
private:
	bool showBattleButton = false;
	int selectedCount = 0;
public:
	cocos2d::Sprite* sprite;
	std::list<Stone*> selectedStones;

	cocos2d::Sprite* battleButton;

	Stone* stones;

	bool getBattleButton();
	void setBattleButton(bool value);

	void setSelectedCount(int value);

	StoneType getRandomType();

	void SelectStone(Stone* stone, bool value);

	StonePanel(cocos2d::Scene* drawScene, std::string fileName, cocos2d::Rect rect,
		cocos2d::Vec2 position, cocos2d::Color3B color);
	//~StonePanel();
};

