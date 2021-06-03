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
	Sprite* panelSprite;

	StoneContainer allStones;
	StoneContainer handStones;
	StoneContainer selectedStones;

	Stone* currentStone;

	StoneContainer::iterator it;

	StonePanel(CharType targetType, string root, Vec2 pos, Size size);
	~StonePanel();

	void InitStones(Size size, string root);

	StoneType GetRandomType();
	StoneTier GetRandomTier();
	int GetRanValueInRange(int front, int rear);

	void SelectStone(int index);
	void UnSelectedStone(int index);

	void HideAll();
	void ShowAll();
	Stone* GetCurrentStone();
	Stone* PopStone();

	void ShowCurrentStone();
	void HideCurrentStone();

	// Just for enemy
	void PushRandomStones(int size);
	void ClearSelectedStone(){selectedStones.clear();}
	void EndBattle();

private:
	CharType target;

	bool firstInit;
	const float actionTime = 0.5f;
};

