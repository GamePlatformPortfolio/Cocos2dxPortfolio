#include "StonePanel.h"

StonePanel::StonePanel(CharacterType target, string fileName, Vec2 spawnPos, Size size)
{
	srand((unsigned int)time(NULL));

	currentStone = nullptr;

	panelSprite = Sprite::create("Images/" + fileName);
	panelSprite->setPosition(spawnPos);
	panelSprite->setContentSize(size);

	this->target = target;

	isFirst = true;
	panelShowTime = 0.5f;

	InitStones(size);
}

StonePanel::~StonePanel()
{
	selectedStones.clear();

	for(int i = 0; i < maxStoneCount; i++)
	{
		delete allStones[i];
	}
}

void StonePanel::InitStones(Size size)
{
	int dividedWidth = size.width / (maxStoneCount + 1);
	int dividedHeight = size.height / 2;

	for (int i = 0; i < maxStoneCount; i++) // 범위 기반 for
	{
		allStones[i] = new Stone
		(
			"Stone.png",
			Vec2(dividedWidth * (i + 1), dividedHeight),
			30,
			rand() % 10
		);
		panelSprite->addChild(allStones[i]->sprite);
	}
	selectedStones.clear();
}

void StonePanel::SelectStone(int index)
{
	allStones[index]->Select(true);

	selectedStones.push_back(allStones[index]);
}

void StonePanel::UnSelectedStone(int index)
{
	list<Stone*>::iterator iter;
	Stone* target = allStones[index];

	iter = find(selectedStones.begin(), selectedStones.end(), target);
	if (iter != selectedStones.end())
	{
		target->Select(false);
		selectedStones.erase(iter);
	}
}

void StonePanel::HideAll()
{
	auto fadeOut = FadeOut::create(panelShowTime);

	panelSprite->runAction(fadeOut->clone());

	for (Stone* stone : allStones)
	{
		stone->Hide();
	}

}

void StonePanel::ShowAll()
{
	auto fadeIn = FadeIn::create(panelShowTime);

	for (Stone* stone : allStones)
	{
		stone->Show();
	}
	panelSprite->runAction(fadeIn->clone());
}

Stone* StonePanel::GetCurrentStone()
{
	if (currentStone != nullptr)
	{
		return currentStone;
	}
	else
	{
		log("@ warning: currentStone is nullptr.");
		return nullptr;
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
		Stone* popStone = &(*(selectedStones.front()));

		selectedStones.pop_front();
		selectedStones.sort();

		return popStone;
	}
}

void StonePanel::ShowCurrentStone()
{
	if (currentStone != nullptr)
		currentStone = nullptr;

	if (target == CharacterType::Enemy && isFirst)
	{
		PushRandomStones();
		isFirst = false;
	}

	for (Stone* stone : allStones)
	{
		stone->ChangeOnlyColor();
	}

	currentStone = PopStone();
	if (currentStone == nullptr)
	{
		EndBattle();
		return;
	}

	Size panelSize = panelSprite->getContentSize();

	// 패널의 중앙으로 이동시킨 후에 표시합니다.
	currentStone->sprite->setPosition(Vec2(panelSize.width / 2, panelSize.height / 2));

	currentStone->Show();
}

void StonePanel::HideCurrentStone()
{
	if (currentStone != nullptr)
	{
		auto hide = CallFunc::create([=]()->void 
			{ 
				currentStone->Hide();
			});

		auto goToOriginPos = CallFunc::create([=]()->void
			{
				currentStone->sprite->setPosition
				(
					currentStone->GetHorizon(),
					currentStone->sprite->getPosition().y
				);
			});

		auto hideSeq = Sequence::create(hide, DelayTime::create(0.5f), goToOriginPos, nullptr);


		currentStone->sprite->runAction(hideSeq);
	}
}

void StonePanel::PushRandomStones()
{
	while(selectedStones.size() != 3)
	{
		int index = rand() % maxStoneCount;

		list<Stone*>::iterator iter;
		Stone* target = allStones[index];

		iter = find(selectedStones.begin(), selectedStones.end(), target);
		if (iter != selectedStones.end())
		{
			continue;
		}
		else
		{
			selectedStones.push_back(target);
			log("@ Log : enemyStone is selected.");
		}
	}
}

void StonePanel::EndBattle()
{
	selectedStones.clear();
	currentStone = nullptr;

	for (Stone* stone : allStones)
	{
		stone->Select(false);
	}

	isFirst = true;
}