//#include "EpPanel.h"
#include "CustomClasses/Character/EpPanel.h"

void EpPanel::Update(UpdateType type) {
	if (stonePanel_target->selectedStones.size() == 0) {
		epText->setString(StringUtils::format("%d", character_target->GetCurrentEp()));
		predEp = 0;
		return;
	}
	predEp = 0;

	for (Stone* index : stonePanel_target->selectedStones) {
	predEp += index->GetEpUsage();
	}

	if(type == UpdateType::Selection) epText->setString(StringUtils::format("%d ( -%d)", character_target->GetCurrentEp(), predEp));
	else if(type == UpdateType::Result) epText->setString(StringUtils::format("%d", character_target->GetCurrentEp()));
	return;
}

void EpPanel::HideAll() {
	auto fadeOut = FadeOut::create(0.5f);

	bgSprite->runAction(fadeOut->clone());
	epText->runAction(fadeOut->clone());
}

void EpPanel::ShowAll() {
	auto fadeIn = FadeIn::create(0.5f);

	bgSprite->runAction(fadeIn->clone());
	epText->runAction(fadeIn->clone());
}