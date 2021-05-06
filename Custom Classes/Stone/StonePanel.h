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

const int maxStoneCount = 6;

class StonePanel
{
public:
	Sprite*			panelSprite;
	list<Stone*>	selectedStones;

	Stone*			allStones[maxStoneCount];
	Stone*			currentStone;

	StonePanel(CharacterType target, string fileName, Vec2 spawnPos, Size size);
	~StonePanel();

	void			InitStones(Size size);

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

