#include "StatusPanel.h"

StatusPanel::StatusPanel(string bgFileName, string hpFileName, 
	Character* character, Size size, Vec2 distance, int hp)
{
	//-----[상태바 생성 및 초기화]
	background = Sprite::create("Images/" + bgFileName);
	background->setContentSize(size);

	target = character;


	// 상태바를 캐릭터에 붙힌다.
	AttachToCharacter(distance);

	background->setColor(Color3B::GRAY);

	//-----



	maxHp = target->GetMaxHp();
	currentHp = maxHp;

	bgSize = background->getContentSize();

	//-----[hp바 생성 및 초기화]
	//hpBar = LoadingBar::create("Images/" + hpFileName);
	//hpBar->setDirection(LoadingBar::Direction::LEFT);

	//originSize = hpBar->getContentSize();

	//background->addChild(hpBar); // hp바의 좌표계의 기준을 상태바의 좌표로 변경

	//hpBar->setContentSize(Size(bgSize.width, bgSize.height / 2));
	//hpBar->setPosition(Vec2(bgSize.width / 2, bgSize.height / 2 + bgSize.height / 4));	
	
	//UpdateHpBar(); // 현재 체력에 따라 hp바 게이지 조절

	//-----


}

StatusPanel::~StatusPanel()
{

}

int StatusPanel::GetMaxHp()
{
	return maxHp;
}

int StatusPanel::GetHp()
{
	return currentHp;
}

void StatusPanel::SetHp(int value)
{
	if (value >= 0)
		currentHp = value;
}

void StatusPanel::UpdateHpBar()
{
	currentHp = target->GetCurrentHp();

	float hpRatio = 100;

	//hpBar->setPercent(hpRatio);
}

void StatusPanel::AttachToCharacter(Vec2 distance)
{
	distanceWithCharacter = distance;

	Vec2 pos;

	switch (target->GetType())
	{
	case CharacterType::Player:
		pos = Vec2(100, target->GetSprite()->getContentSize().height / 2);
		background->setPosition(pos);
		break;
	case CharacterType::Enemy:
		background->setPosition(target->GetSprite()->getPosition() + distance);
		break;
	}
	target->GetSprite()->addChild(background);
}

CharacterType StatusPanel::GetTarget()
{
	return CharacterType::None;
}