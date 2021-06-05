#pragma once
#include "cocos2d.h"
#include "CustomClasses/EnumCollection/EnumCollection.h"
#include <random>

using namespace cocos2d;
using namespace std;

class Stone
{
public:
	Stone(StoneType type, StoneTier tier, string root, Vec2 pos, int size);
	~Stone();

	void SetPowerAndEpUsage();
	void InitSprite(string root);
	
	Sprite* GetSprite();

	int GetRanValueInRange(int, int);

	void ChangeState();
	void ChangeOnlyColor();

	void Select(bool value);
	bool GetSelect();

	int	GetPower();
	StoneType GetType();
	int GetEpUsage();

	void Show();
	void Hide();

	void ShowPower();
	void HidePower();

	void InitInformSprite();
	void ShowInform();
	void HideInform();

#pragma region PRIVATE
private:
	Sprite* sprite;
	Label* powerText;

	Sprite* informSprite;

	const Color3B color = Color3B::WHITE;
	const Color3B selectedColor = Color3B::GRAY;

	StoneType type;
	StoneTier tier;

	int epUsage;

	int power;
	const int fontSize = 24;
	const float actionTime = 0.2f;

	bool isSelect;
#pragma endregion
};

