#pragma once

#include "Custom Classes/Stone/Stone.h"
#include "Custom Classes/Enum Collection/EnumCollection.h"
#include "cocos2d.h"
#include <list>
#include <iostream>
#include <cstdlib>
#include <ctime>

using namespace cocos2d;
using namespace std;

const int maxAmount = 24;
const int handAmount = 6;

typedef vector<Stone*> StoneContainer;

class StonePanel
{
public:
	Sprite*			panelSprite;

	StoneContainer	allStones;
	StoneContainer	handStones;
	StoneContainer	selectedStones;

	Stone*			currentStone;

	StoneContainer::iterator  it;

	StonePanel(CharacterType target, string fileName, Vec2 spawnPos, cocos2d::Size size);
	~StonePanel();

	void			InitStones(cocos2d::Size size);

	StoneType		GetRandomType();
	StoneTier		GetRandomTier();
	int GetRanValueInRange(int front, int rear);

	void			SelectStone(int index);
	void			UnSelectedStone(int index);

	void			HideAll();
	void			ShowAll();
	Stone*			GetCurrentStone();
	Stone*			PopStone();

	void			ShowCurrentStone();
	void			HideCurrentStone();
	void			PushRandomStones();
	void			EndBattle();

private:
	CharacterType	target;

	bool			isFirst;
	float			panelShowTime;
};

