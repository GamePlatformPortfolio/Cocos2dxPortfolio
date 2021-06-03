#include "StonePanel.h"
#include "Custom Classes/Enum Collection/EnumCollection.h"

StonePanel::StonePanel(CharType targetType, string root, Vec2 pos, Size size)
{
	currentStone = nullptr;

	panelSprite = Sprite::create(root + "Panel/StonePanel.png");
	panelSprite->setPosition(pos);
	panelSprite->setContentSize(size);

	this->target = targetType;

	firstInit = true;

	InitStones(size, root);
}

StonePanel::~StonePanel()
{

}

void StonePanel::InitStones(Size size, string root)
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
			root,
			Vec2::ZERO,
			50
		));
		allStones[i]->stoneImg->setVisible(false);
		panelSprite->addChild(allStones[i]->stoneImg);
  }

	selectedStones.clear();
	handStones.clear();

	// handStones에 원소 넣기
	for (int i = 0; i < handAmount; i++)
	{
		handStones.push_back(allStones[i]);
		handStones[i]->stoneImg->setVisible(true);
		handStones[i]->stoneImg->setPosition(Vec2(hOffset * (i + 1), vOffset)); // hOffset * 1 부터 시작해야 함
	}	
}

void StonePanel::SelectStone(int index)
{
	handStones[index]->state = StoneState::SELECTED;
	handStones[index]->UpdateStoneState();

	selectedStones.push_back(handStones[index]);
}

void StonePanel::UnSelectedStone(int index)
{
	Stone* target = handStones[index];

	// 반복자를 통한 탐색. 현재 클릭한 스톤이 이미 선택되었는지 확인합니다.
	it = find(selectedStones.begin(), selectedStones.end(), target);
	if (it != selectedStones.end())
	{
		target->state = StoneState::NONE_SELECTED;
		target->UpdateStoneState();

		selectedStones.erase(it);
	}
}

void StonePanel::HideAll()
{
	auto fadeOut = FadeOut::create(DEFAULT_ACTION_TIME);

	// 패널을 숨깁니다.
	panelSprite->runAction(fadeOut->clone());

	// 손에 든 모든 스톤을 숨깁니다.
	for (Stone* stone : handStones)
	{
		stone->HideStone();
	}
}

void StonePanel::ShowAll()
{
	auto fadeIn = FadeIn::create(DEFAULT_ACTION_TIME);

	// 패널을 보입니다.
	panelSprite->runAction(fadeIn->clone());

	const int hOffset = panelSprite->getContentSize().width / (handAmount + 1);
	const int vOffset = panelSprite->getContentSize().height / 2;

	selectedStones.clear();
	handStones.clear();

	for (Stone* stone : allStones)
	{
		stone->state = StoneState::NONE_SELECTED;
	}

	// 손에 든 모든 스톤을 보입니다.
	for (int i = 0; i < handAmount; i++)
	{
		handStones.push_back(allStones[i]);		
		handStones[i]->stoneImg->setPosition(Vec2(hOffset * (i + 1), vOffset)); // hOffset * 1 부터 시작해야 함
	}

	for (Stone* stone : handStones)
	{
		stone->stoneImg->setVisible(true);
		stone->ShowStone(false);
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
	// 스톤들의 상태는 변경하지 않고 색만 바꿉니다.
	for (Stone* stone : handStones)
	{
		stone->ChangeColorOnly();
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
	currentStone->stoneImg->setPosition(targetPos);
	currentStone->ShowStone(true);
}

void StonePanel::HideCurrentStone()
{
	// CurrentStone은 selectedStones에서 pop된 상태...hand와 all에서 없애야 합니다.

	if (currentStone != nullptr)
	{
		auto hide = CallFunc::create([=]()->void 
			{ 
				currentStone->HideStone();
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

		currentStone->stoneImg->runAction(hideSeq);
	}
}

void StonePanel::PushRandomStones(int size)
{
	while(selectedStones.size() != size)
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
	firstInit = true;
}

StoneType StonePanel::GetRandomType() 
{
	switch (GetRanValueInRange(0,3)) 
	{
		case 0: return StoneType::PHYSICAL_ATTACK;
		case 1: return StoneType::MAGIC_ATTACK;
		case 2: return StoneType::GUARD;
		case 3: return StoneType::DODGE;
	}
}

StoneTier StonePanel::GetRandomTier() 
{
	switch (GetRanValueInRange(0, 3)) 
	{
		case 0: return StoneTier::NORMAL;
		case 1: return StoneTier::RARE;
		case 2: return StoneTier::UNIQUE;
		case 3: return StoneTier::EPIC;
	}
}

int StonePanel::GetRanValueInRange(int front, int rear) 
{
	std::random_device rd;
	std::mt19937_64 gen(rd());
	std::uniform_int_distribution<int> dis(front, rear);
	return dis(gen);
}
