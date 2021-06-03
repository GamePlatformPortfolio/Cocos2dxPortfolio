#pragma once

#include <random>

#include "cocos2d.h"
#include "Custom Classes/Enum Collection/EnumCollection.h"

using namespace cocos2d;
using namespace std;

#define NORMAL_COLOR					Color3B::WHITE
#define SELECT_COLOR					Color3B::GRAY

#define DEFAULT_ACTION_TIME				0.2f
#define DEFAULT_FONT_SIZE				24
#define DEFAULT_INFORM_VERTICAL_OFFSET  75

typedef enum StoneState { NONE_SELECTED, SELECTED };

typedef struct Stone
{
#pragma region Variable
	Sprite* stoneImg;
	Sprite* stoneInformImg;

	Label* power;
	int powerValue;

	StoneState state;

	// StoneType, StoneTier, epUsage : O - pera
	StoneType type;
	StoneTier tier;

	int epUsage;
#pragma endregion

#pragma region Function
	Stone(StoneType type, StoneTier tier, string root, Vec2 pos, int size);
	~Stone();

	// InitSprite, GetRanValueInRange, SetPowerAndEpUsage : O - pera
	void InitSprite(string root);

	int GetRanValueInRange(int, int);

	void SetPowerAndEpUsage();
	// --------------------

	void UpdateStoneState();
	void ChangeColorOnly();

	void ShowStone(bool showPower);
	void HideStone();

	void InitInformImg();

#pragma endregion
};

