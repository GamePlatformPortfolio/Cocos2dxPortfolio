#include "Stone.h"
#include "StoneTier.h"
#include "StoneType.h"

Stone::Stone(StoneType getType, StoneTier getTier, Vec2 spawnPos, int size):
	type(getType), tier(getTier)
{
	//--------------스프라이트 생성 / 세팅
	SetSprite();			
	sprite->setContentSize(Size(size, size));
	sprite->setPosition(spawnPos);

	//--------------등급별 주사위값 세팅
	//srand((unsigned int)time(NULL));
	SetPowerAndEpUsage();

	//활성화/비활성화 상태 색 설정
	color = Color3B::WHITE;
	selectedColor = Color3B::GRAY;

	this->power = power;
	powerFontSize = 24;
	powerShowTime = 0.2f;
	isSelect = false;

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



void Stone::SetPowerAndEpUsage() {
	if (type == StoneType::Guard) {
		switch (tier) {
		case StoneTier::Normal: {
			power = GetRanValueInRange(1, 4);
			epUsage = 1;
			break;
		}
		case StoneTier::Rare: {
			power = GetRanValueInRange(2, 6);
			epUsage = 2;
			break;
		}
		case StoneTier::Unique: {
			power = GetRanValueInRange(3, 6);
			epUsage = 3;
			break;
		}
		case StoneTier::Epic: {
			power = GetRanValueInRange(4, 8);
			epUsage = 4;
			break;
		}
		}
	}
	else if (type == StoneType::Dodge) {
		switch (tier) {
		case StoneTier::Normal: {
			power = GetRanValueInRange(1, 6);
			epUsage = 1;
			break;
		}
		case StoneTier::Rare: {
			power = GetRanValueInRange(2, 6);
			epUsage = 2;
			break;
		}
		case StoneTier::Unique: {
			power = GetRanValueInRange(2, 8);
			epUsage = 3;
			break;
		}
		case StoneTier::Epic: {
			power = GetRanValueInRange(4, 8);
			epUsage = 5;
			break;
		}
		}
	}
	else {
		switch (tier) {
		case StoneTier::Normal: {
			power = GetRanValueInRange(1, 5);
			epUsage = 1;
			break;
		}
		case StoneTier::Rare: {
			power = GetRanValueInRange(1, 8);
			epUsage = 2;
			break;
		}
		case StoneTier::Unique: {
			power = GetRanValueInRange(3, 8);
			epUsage = 3;
			break;
		}
		case StoneTier::Epic: {
			power = GetRanValueInRange(5, 10);
			epUsage = 4;
			break;
		}
		}
	}
}

void Stone::SetSprite() {
	string filename = "Stn_";
	//----------------------스톤 타입(물공, 마공, 방어, 회피)
	if		(type == StoneType::PhysicalAttack)		filename += StringUtils::format("PA_");
	else if (type == StoneType::MagicAttack)		filename += StringUtils::format("MA_");
	else if (type == StoneType::Guard)				filename += StringUtils::format("GD_");
	else if (type == StoneType::Dodge)				filename += StringUtils::format("DG_");
	//----------------------스톤 등급(Normal, Rare, Unique, Epic)
	if		(tier == StoneTier::Normal)				filename += StringUtils::format("N.png");
	else if (tier == StoneTier::Rare)				filename += StringUtils::format("R.png");
	else if (tier == StoneTier::Unique)				filename += StringUtils::format("U.png");
	else if (tier == StoneTier::Epic)				filename += StringUtils::format("E.png");
	//------------최종 결과물 예시 : "Stn_PA_N.png"

		//스프라이트 생성 / 세팅
	sprite = Sprite::create(filename);
}

//Return Random Value between front and rear
int Stone::GetRanValueInRange(int front, int rear) {
	//return (rand() % (rear - front + 1)) + front;
	std::random_device rd;
	std::mt19937_64 gen(rd());
	std::uniform_int_distribution<int> dis(front, rear);
	return dis(gen);
}

bool Stone::GetSelect()
{
	return isSelect;
}

float Stone::GetHorizon()
{
	return isSelect;
}

int Stone::GetPower()
{
	return power;
}