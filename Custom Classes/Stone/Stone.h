#pragma once
#include "cocos2d.h"
#include "StoneTier.h"
#include "StoneType.h"
#include <random>

using namespace cocos2d;
using namespace std;

class Stone
{
public:
	Sprite* sprite;
	Label*  powerLabel;
	Color3B color;
	Color3B selectedColor;

	Stone(StoneType getType, StoneTier getTier, Vec2 spawnPos, int size);
	~Stone();

	void	ChangeState();
	void	ChangeOnlyColor();

	void    Select(bool value);
	bool	GetSelect();

	void    SetPowerAndEpUsage();
	void	SetSprite();

	int		GetPower();

	void	Show();
	void	Hide();

	void	ShowPowerLabel();
	void	HidePowerLanel();

	float	GetHorizon();
	int		GetRanValueInRange(int, int);
private:
	StoneType type;
	StoneTier tier;
	int		epUsage;
	int		power;
	int		powerFontSize;
	float	powerShowTime;
	bool	isSelect;	
	float   horizon;
};

