#pragma once
#include "cocos2d.h"
#include "CustomClasses/Stone/StonePanel.h"
#include "CustomClasses/Character/Character.h"

class EpPanel {
private:
	cocos2d::Sprite* bgSprite;
	cocos2d::Label* epText;
	StonePanel* stonePanel_target;
	Character* character_target;
	

public:
	EpPanel(StonePanel* panel, Character* character) {
		stonePanel_target = panel;
		character_target = character;
		predEp = 0;
		bgSprite = cocos2d::Sprite::create("Images/Panel/EPanel.png");
		epText = cocos2d::Label::createWithSystemFont("", "fonts/Schwarzwald_Regular.ttf", 25);
		bgSprite->addChild(epText);
		epText->setPosition(Vec2(bgSprite->getContentSize().width/2, bgSprite->getContentSize().height/2));
		Update(UpdateType::Selection);
		bgSprite->setPosition(Vec2(panel->panelSprite->getPosition().x, panel->panelSprite->getPosition().y + 50));
	}
	void Update(UpdateType type);
	cocos2d::Sprite* GetSprite() noexcept { return bgSprite; };
	int GetPredEP() noexcept { return predEp; }
	void HideAll();
	void ShowAll();
	int predEp = 0;
};