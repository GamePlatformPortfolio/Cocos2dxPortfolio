#include "Custom Classes/StatPanel/StatPanel.h"

StatPanel::StatPanel(CharType type, string root) {
	//Set StatusPanel Image by type
	switch (type) {
	case CharType::PLAYER: {
		statBackG = Sprite::create(root + "StatPanel/Crv_StatBar.png");
		hpGauge = LoadingBar::create(root + "StatPanel/hpBar.png");
		npGauge = LoadingBar::create(root + "StatPanel/npBar.png");
		hpGauge->setDirection(LoadingBar::Direction::RIGHT);
		npGauge->setDirection(LoadingBar::Direction::LEFT);
	}break;
	case CharType::ENEMY: {
		statBackG = Sprite::create(root + "StatPanel/Enemy_StatBar.png");
		hpGauge = LoadingBar::create(root + "StatPanel/hpBar.png");
		npGauge = LoadingBar::create(root + "StatPanel/npBar.png");
		hpGauge->setDirection(LoadingBar::Direction::RIGHT);
		npGauge->setDirection(LoadingBar::Direction::LEFT);
	}break;
	default:	break;
	}
	//target = newTarget;
	UpdateStat();
}
void StatPanel::UpdateStat() {
	hpGauge->setPercent(100);
	npGauge->setPercent(100);
}
void StatPanel::SetHpGauge(int maxHp, int currentHp) {
	hpGauge->setPercent(((float)currentHp / (float)maxHp) * 100);
}
void StatPanel::SetNpGauge(int maxNp, int currentNp) {
	npGauge->setPercent(((float)currentNp / (float)maxNp) * 100);
}