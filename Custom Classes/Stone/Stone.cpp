#include "Stone.h"
#include "StoneTier.h"
#include "StoneType.h"

Stone::Stone(StoneType getType, StoneTier getTier, Vec2 spawnPos, int size, int power)
{
	string filename = "Stn_";
	//----------------------스톤 타입(물공, 마공, 방어, 회피)
	if		(getType == StoneType::PhysicalAttack)	filename += StringUtils::format("PA_");
	else if (getType == StoneType::MagicAttack)		filename += StringUtils::format("MA_");
	else if (getType == StoneType::Guard)			filename += StringUtils::format("GD_");
	else if (getType == StoneType::Dodge)			filename += StringUtils::format("DG_");
	//----------------------스톤 등급(Normal, Rare, Unique, Epic)
	if (getTier == StoneTier::Normal)			filename += StringUtils::format("N.png");
	else if (getTier == StoneTier::Rare)		filename += StringUtils::format("R.png");
	else if (getTier == StoneTier::Unique)		filename += StringUtils::format("U.png");
	else if (getTier == StoneTier::Epic)		filename += StringUtils::format("E.png");
	//------------최종 결과물 예시 : "Stn_PA_N.png"
	sprite = Sprite::create(filename);

	sprite->setContentSize(Size(size, size));
	sprite->setPosition(spawnPos);

	color = Color3B::WHITE;
	selectedColor = Color3B::GRAY;

	this->power = power;
	powerFontSize = 24;
	powerShowTime = 0.2f;
	isSelect = false;
	horizon = spawnPos.x;

	powerLabel = Label::createWithSystemFont(to_string(power), "", powerFontSize);
	powerLabel->setColor(Color3B::BLACK);
	powerLabel->setPosition(Vec2(size / 2, size / 2));
	sprite->addChild(powerLabel);

	ChangeState();
}

Stone::~Stone()
{

}

void Stone::ChangeState()
{
	if (isSelect)
		sprite->setColor(selectedColor);
	else
	{
		sprite->setColor(color);
	}
}

void Stone::ChangeOnlyColor()
{
	sprite->setColor(color);
}

void Stone::Select(bool value)
{
	isSelect = value;
	ChangeState();
}

bool Stone::GetSelect()
{
	return isSelect;
}

int Stone::GetPower()
{
	return power;
}

void Stone::Show()
{
	auto fadeIn = FadeIn::create(powerShowTime);
	sprite->runAction(fadeIn->clone());
	powerLabel->runAction(fadeIn->clone());
}

void Stone::Hide()
{
	auto fadeOut = FadeOut::create(powerShowTime);
	sprite->runAction(fadeOut->clone());
	powerLabel->runAction(fadeOut->clone());
}

void Stone::ShowPowerLabel()
{
	auto fadeIn = FadeIn::create(powerShowTime);
	powerLabel->runAction(fadeIn);
}

void Stone::HidePowerLanel()
{
	auto fadeOut = FadeOut::create(powerShowTime);
	powerLabel->runAction(fadeOut);
}

float Stone::GetHorizon()
{
	return horizon;
}