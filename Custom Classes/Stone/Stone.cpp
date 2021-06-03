#include "Stone.h"

#pragma region Function
Stone::Stone(StoneType type, StoneTier tier, string root, Vec2 pos, int size)
{
	this->type = type;
	this->tier = tier;

	// 이미지 초기화
	InitSprite(root);
	stoneImg->setContentSize(Size(size, size));
	stoneImg->setPosition(pos);

	// 등급별 주사위값 설정
	SetPowerAndEpUsage();

	// 활성화/비활성화 상태 색 설정
	this->powerValue = powerValue;
	this->state = StoneState::NONE_SELECTED;

	power = Label::createWithSystemFont(to_string(powerValue), "", DEFAULT_FONT_SIZE);
	power->setColor(Color3B::BLACK);
	power->setPosition(Vec2(size / 2, size / 2));
	power->setVisible(false);

	stoneImg->addChild(power);

	UpdateStoneState();
}

Stone::~Stone()
{

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
	stoneImg = Sprite::create(root + "Stone/" + filename);

	InitInformImg();
}

int Stone::GetRanValueInRange(int front, int rear)
{
	std::random_device rd;
	std::mt19937_64 gen(rd());
	std::uniform_int_distribution<int> dis(front, rear);
	return dis(gen);
}

void Stone::SetPowerAndEpUsage() {
	if (type == StoneType::GUARD) {
		switch (tier) {
		case StoneTier::NORMAL: {
			powerValue = GetRanValueInRange(1, 4);
			epUsage = 1;
			break;
		}
		case StoneTier::RARE: {
			powerValue = GetRanValueInRange(2, 6);
			epUsage = 2;
			break;
		}
		case StoneTier::UNIQUE: {
			powerValue = GetRanValueInRange(3, 6);
			epUsage = 3;
			break;
		}
		case StoneTier::EPIC: {
			powerValue = GetRanValueInRange(4, 8);
			epUsage = 4;
			break;
		}
		}
	}
	else if (type == StoneType::DODGE) {
		switch (tier) {
		case StoneTier::NORMAL: {
			powerValue = GetRanValueInRange(1, 6);
			epUsage = 1;
			break;
		}
		case StoneTier::RARE: {
			powerValue = GetRanValueInRange(2, 6);
			epUsage = 2;
			break;
		}
		case StoneTier::UNIQUE: {
			powerValue = GetRanValueInRange(2, 8);
			epUsage = 3;
			break;
		}
		case StoneTier::EPIC: {
			powerValue = GetRanValueInRange(4, 8);
			epUsage = 5;
			break;
		}
		}
	}
	else {
		switch (tier) {
		case StoneTier::NORMAL: {
			powerValue = GetRanValueInRange(1, 5);
			epUsage = 1;
			break;
		}
		case StoneTier::RARE: {
			powerValue = GetRanValueInRange(1, 8);
			epUsage = 2;
			break;
		}
		case StoneTier::UNIQUE: {
			powerValue = GetRanValueInRange(3, 8);
			epUsage = 3;
			break;
		}
		case StoneTier::EPIC: {
			powerValue = GetRanValueInRange(5, 10);
			epUsage = 4;
			break;
		}
		}
	}
}

void Stone::UpdateStoneState()
{
	switch (state)
	{
	case StoneState::NONE_SELECTED:
		stoneImg->setColor(NORMAL_COLOR);
		break;
	case StoneState::SELECTED:
		stoneImg->setColor(SELECT_COLOR);
		break;
	}
}

void Stone::ChangeColorOnly()
{
	stoneImg->setColor(NORMAL_COLOR);
}

void Stone::ShowStone(bool showPower)
{
	auto fadeIn = FadeIn::create(DEFAULT_ACTION_TIME);

	stoneImg->runAction(fadeIn);

	if (showPower)
	{
		power->setVisible(true);
		power->runAction(fadeIn->clone());
	}
	else
		power->setVisible(false);
}

void Stone::HideStone()
{
	// 이미지도 텍스트도 가리고
	auto fadeOut = FadeOut::create(DEFAULT_ACTION_TIME);

	stoneImg->runAction(fadeOut);

	if (power->isVisible())
		power->runAction(fadeOut->clone());
}

void Stone::InitInformImg()
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

	stoneInformImg = Sprite::create("Images/Value/" + name);
	stoneInformImg->setVisible(false);

	stoneImg->addChild(stoneInformImg);

	stoneInformImg->setPosition(Vec2(0, DEFAULT_INFORM_VERTICAL_OFFSET));
}
#pragma endregion

































