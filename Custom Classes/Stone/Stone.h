#pragma once
#include "cocos2d.h"
#include "StoneTier.h"
#include "StoneType.h"

using namespace cocos2d;
using namespace std;

class Stone
{
public:
	Sprite* sprite;
	Label*  powerLabel;
	Color3B color;
	Color3B selectedColor;

	Stone(StoneType getType, StoneTier getTier, Vec2 spawnPos, int size, int power);
	~Stone();

	void	ChangeState();
	void	ChangeOnlyColor();

	void    Select(bool value);
	bool	GetSelect();

	int		GetPower();

	void	Show();
	void	Hide();

	void	ShowPowerLabel();
	void	HidePowerLanel();

	float	GetHorizon();

private:
	int		power;
	int		powerFontSize;
	float	powerShowTime;
	bool	isSelect;	
	float   horizon;
};

