#include "Stone.h"

Stone::Stone(string fileName, Vec2 spawnPos, int size, int power)
{
	sprite = Sprite::create("Images/" + fileName);
	sprite->setContentSize(Size(size, size));
	sprite->setPosition(spawnPos);

	color = Color3B::WHITE;
	selectedColor = Color3B::GRAY;

	this->power = power;
	powerFontSize = 24;
	powerShowTime = 0.5f;
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