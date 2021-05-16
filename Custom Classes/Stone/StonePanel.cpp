#include "StonePanel.h"
#include "StoneType.h"
#include "StoneTier.h"

StonePanel::StonePanel(CharacterType target, string fileName, Vec2 spawnPos, Size size)
{
	currentStone = nullptr;

	panelSprite = Sprite::create(fileName);
	panelSprite->setPosition(spawnPos);
	panelSprite->setContentSize(size);

	this->target = target;

	isFirst = true;
	panelShowTime = 0.5f;

	InitStones(size);
}

StonePanel::~StonePanel()
{

}

void StonePanel::InitStones(Size size)
{
	const int hOffset = size.width / (handAmount + 1); // handAmount + 1로 나눠야 handStones을(를) 대칭으로 배치가능
	const int vOffset = size.height / 2;

	// allStones에 원소 넣기
	for (int i = 0; i < maxAmount; i++)
	{
		allStones.push_back(new Stone
		(
			GetRandomType(),
			GetRandomTier(),
			Vec2::ZERO,
			50
		));
		allStones[i]->sprite->setVisible(false);
		panelSprite->addChild(allStones[i]->sprite);
	}

	// selectedStones 비우기
	selectedStones.clear();
	handStones.clear();

	// handStones에 원소 넣기
	for (int i = 0; i < handAmount; i++)
	{
		handStones.push_back(allStones[i]);
		handStones[i]->sprite->setVisible(true);
		handStones[i]->sprite->setPosition(Vec2(hOffset * (i + 1), vOffset)); // hOffset * 1 부터 시작해야 함
	}	
}

void StonePanel::SelectStone(int index)
{
	handStones[index]->Select(true);

	selectedStones.push_back(handStones[index]);
}

void StonePanel::UnSelectedStone(int index)
{
	Stone* target = handStones[index];

	// 반복자를 통한 탐색. 현재 클릭한 스톤이 이미 선택되었는지 확인합니다.
	it = find(selectedStones.begin(), selectedStones.end(), target);
	if (it != selectedStones.end())
	{
		target->Select(false);

		selectedStones.erase(it);
	}
}

void StonePanel::HideAll()
{
	auto fadeOut = FadeOut::create(panelShowTime);

	// 패널을 숨깁니다.
	panelSprite->runAction(fadeOut->clone());

	// 손에 든 모든 스톤을 숨깁니다.
	for (Stone* stone : handStones)
	{
		stone->Hide();
	}
}

void StonePanel::ShowAll()
{
	auto fadeIn = FadeIn::create(panelShowTime);

	// 패널을 보입니다.
	panelSprite->runAction(fadeIn->clone());

	const int hOffset = panelSprite->getContentSize().width / (handAmount + 1);
	const int vOffset = panelSprite->getContentSize().height / 2;

	selectedStones.clear();
	handStones.clear();

	// 손에 든 모든 스톤을 보입니다.
	for (int i = 0; i < handAmount; i++)
	{
		allStones[i]->Show();
		handStones.push_back(allStones[i]);

		handStones[i]->sprite->setPosition(Vec2(hOffset * (i + 1), vOffset)); // hOffset * 1 부터 시작해야 함

		
	
	}

	for (Stone* stone : allStones)
	{
		stone->Select(false);
	}

	for (Stone* stone : handStones)
	{
		stone->sprite->setVisible(true);
	}
}

Stone* StonePanel::GetCurrentStone()
{
	if (currentStone != nullptr)
	{
		return currentStone;
	}
}

Stone* StonePanel::PopStone()
{
	if (selectedStones.empty())
	{
		return nullptr;
	}
	else
	{
		it = selectedStones.begin();
		Stone* stone = &(*(selectedStones.front()));

		selectedStones.erase(it);

		return stone;
	}
}

void StonePanel::ShowCurrentStone()
{
	// 상대방의 패널이면 랜덤한 스톤을 선택합니다.
	if (target == CharacterType::Enemy && isFirst)
	{
		PushRandomStones();
		isFirst = false;
	}

	// 스톤들의 상태는 변경하지 않고 색만 바꿉니다.
	for (Stone* stone : handStones)
	{
		stone->ChangeOnlyColor();
	}

	// 선택된 스톤들 중 맨 앞 스톤을 추출합니다.
	currentStone = PopStone();

	if (currentStone == nullptr)
	{
		EndBattle();
		return;
	}

	Vec2 targetPos = Vec2(panelSprite->getContentSize().width / 2, panelSprite->getContentSize().height / 2);

	// 패널의 중앙으로 이동시킨 후에 표시합니다.
	currentStone->sprite->setPosition(targetPos);
	currentStone->Show();
}

void StonePanel::HideCurrentStone()
{
	// CurrentStone은 selectedStones에서 pop된 상태...hand와 all에서 없애야 합니다.

	if (currentStone != nullptr)
	{
		auto hide = CallFunc::create([=]()->void 
			{ 
				currentStone->Hide();
			});

		auto moveToLast = CallFunc::create([=]()->void
			{
				it = find(allStones.begin(), allStones.end(), currentStone);
				if (it != allStones.end())
				{
					allStones.erase(it); // 먼저 allStones에서 제거합니다.

					allStones.push_back(currentStone);
				}
			});

		auto hideSeq = Sequence::create(hide, DelayTime::create(0.2f), moveToLast, nullptr);

		currentStone->sprite->runAction(hideSeq);		
	}
}

void StonePanel::PushRandomStones()
{
	while(selectedStones.size() != 3)
	{
		int index = rand() % handAmount;

		Stone* target = allStones[index];

		it = find(selectedStones.begin(), selectedStones.end(), target);
		if (it != selectedStones.end())
		{
			continue;
		}
		else
		{
			selectedStones.push_back(target);
		}
	}
}

void StonePanel::EndBattle()
{

	


	isFirst = true;
}

StoneType StonePanel::GetRandomType() {
	//srand(time(NULL));
	switch (GetRanValueInRange(0,3)) {
	case 0: return StoneType::PhysicalAttack;
	case 1: return StoneType::MagicAttack;
	case 2: return StoneType::Guard;
	case 3: return StoneType::Dodge;
	}
}

StoneTier StonePanel::GetRandomTier() {
	//srand(time(NULL));
	switch (GetRanValueInRange(0, 3)) {
	case 0: return StoneTier::Normal;
	case 1: return StoneTier::Rare;
	case 2: return StoneTier::Unique;
	case 3: return StoneTier::Epic;
	}
}

int StonePanel::GetRanValueInRange(int front, int rear) {
	//return (rand() % (rear - front + 1)) + front;
	std::random_device rd;
	std::mt19937_64 gen(rd());
	std::uniform_int_distribution<int> dis(front, rear);
	return dis(gen);
}
