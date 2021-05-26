#include "Stone.h"
#include "Custom Classes/Enum Collection/EnumCollection.h"

Stone::Stone(StoneType type, StoneTier tier, string root, Vec2 pos, int size)
{
	this->type = type;
	this->tier = tier;

	//--------------스프라이트 생성 / 세팅
	InitSprite(root);			
	sprite->setContentSize(Size(size, size));
	sprite->setPosition(pos);

	//--------------등급별 주사위값 세팅
	SetPowerAndEpUsage();

	//활성화/비활성화 상태 색 설정
	this->power = power;
	this->isSelect = false;

	powerText = Label::createWithSystemFont(to_string(power), "", fontSize);
	powerText->setColor(Color3B::BLACK);
	powerText->setPosition(Vec2(size / 2, size / 2));
	sprite->addChild(powerText);

	ChangeState();
}

Stone::~Stone()
{

}

Sprite* Stone::GetSprite()
{
	return sprite;
}

//Return Random Value between front and rear
int Stone::GetRanValueInRange(int front, int rear) {
	//return (rand() % (rear - front + 1)) + front;
	std::random_device rd;
	std::mt19937_64 gen(rd());
	std::uniform_int_distribution<int> dis(front, rear);
	return dis(gen);
}

void Stone::InitSprite(string root) 
{
	string filename = "Stn_";
	//----------------------스톤 타입(물공, 마공, 방어, 회피)
	if (type == StoneType::PHYSICAL_ATTACK)		filename += StringUtils::format("PA_");
	else if (type == StoneType::MAGIC_ATTACK)	filename += StringUtils::format("MA_");
	else if (type == StoneType::GUARD)			filename += StringUtils::format("GD_");
	else if (type == StoneType::DODGE)			filename += StringUtils::format("DG_");
	//----------------------스톤 등급(Normal, Rare, Unique, Epic)
	if (tier == StoneTier::NORMAL)				filename += StringUtils::format("N.png");
	else if (tier == StoneTier::RARE)			filename += StringUtils::format("R.png");
	else if (tier == StoneTier::UNIQUE)			filename += StringUtils::format("U.png");
	else if (tier == StoneTier::EPIC)			filename += StringUtils::format("E.png");
	//------------최종 결과물 예시 : "Stn_PA_N.png"

	//스프라이트 생성 / 세팅
	sprite = Sprite::create(root + "Stone/" + filename);

	InitInformSprite();

}

void Stone::SetPowerAndEpUsage() {
	if (type == StoneType::GUARD) {
		switch (tier) {
		case StoneTier::NORMAL: {
			power = GetRanValueInRange(1, 4);
			epUsage = 1;
			break;
		}
		case StoneTier::RARE: {
			power = GetRanValueInRange(2, 6);
			epUsage = 2;
			break;
		}
		case StoneTier::UNIQUE: {
			power = GetRanValueInRange(3, 6);
			epUsage = 3;
			break;
		}
		case StoneTier::EPIC: {
			power = GetRanValueInRange(4, 8);
			epUsage = 4;
			break;
		}
		}
	}
	else if (type == StoneType::DODGE) {
		switch (tier) {
		case StoneTier::NORMAL: {
			power = GetRanValueInRange(1, 6);
			epUsage = 1;
			break;
		}
		case StoneTier::RARE: {
			power = GetRanValueInRange(2, 6);
			epUsage = 2;
			break;
		}
		case StoneTier::UNIQUE: {
			power = GetRanValueInRange(2, 8);
			epUsage = 3;
			break;
		}
		case StoneTier::EPIC: {
			power = GetRanValueInRange(4, 8);
			epUsage = 5;
			break;
		}
		}
	}
	else {
		switch (tier) {
		case StoneTier::NORMAL: {
			power = GetRanValueInRange(1, 5);
			epUsage = 1;
			break;
		}
		case StoneTier::RARE: {
			power = GetRanValueInRange(1, 8);
			epUsage = 2;
			break;
		}
		case StoneTier::UNIQUE: {
			power = GetRanValueInRange(3, 8);
			epUsage = 3;
			break;
		}
		case StoneTier::EPIC: {
			power = GetRanValueInRange(5, 10);
			epUsage = 4;
			break;
		}
		}
	}
}

void Stone::ChangeState()
{
	if (isSelect)
	{
		sprite->setColor(selectedColor);
	}
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

StoneType Stone::GetType()
{
	return type;
}

void Stone::Show()
{
	auto fadeIn = FadeIn::create(actionTime);

	sprite->runAction(fadeIn->clone());
	powerText->runAction(fadeIn->clone());
}

void Stone::Hide()
{
	auto fadeOut = FadeOut::create(actionTime);

	sprite->runAction(fadeOut->clone());
	powerText->runAction(fadeOut->clone());
}

void Stone::ShowPower()
{
	auto fadeIn = FadeIn::create(actionTime);

	powerText->runAction(fadeIn);
}

void Stone::HidePower()
{
	auto fadeOut = FadeOut::create(actionTime);

	powerText->runAction(fadeOut);
}

void Stone::InitInformSprite()
{
	string name = "";

	switch (type)
	{
	case PHYSICAL_ATTACK:
		name = "PA";
		break;
	case MAGIC_ATTACK:
		name = "MA";
		break;
	case GUARD:
		name = "GD";
		break;
	case DODGE:
		name = "DG";
		break;
	}

	name += "UI_";

	switch (tier)
	{
	case NORMAL:
		name += "N";
		break;
	case RARE:
		name += "R";
		break;
	case UNIQUE:
		name += "U";
		break;
	case EPIC:
		name += "E";
		break;
	}

	name += ".png";

	informSprite = Sprite::create("Images/Value/" + name);
	informSprite->setVisible(false);

	sprite->addChild(informSprite);


	informSprite->setPosition(Vec2(0, 75));
}

void Stone::ShowInform()
{
	informSprite->setVisible(true);
}

void Stone::HideInform()
{
	informSprite->setVisible(false);
}









