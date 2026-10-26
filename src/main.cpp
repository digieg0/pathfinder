#include <Geode/Geode.hpp>
#include <Geode/modify/MenuLayer.hpp>

#include "ui/Menu.hpp"

using namespace geode::prelude;

class $modify(PathfinderMenuLayer, MenuLayer) {
	bool init() {
		if (!MenuLayer::init()) return false;

		static bool imguiReady = false;
		if (!imguiReady) {
			PathfinderMenu::install();
			imguiReady = true;
		}

		auto menu = this->getChildByID("bottom-menu");
		if (!menu || menu->getChildByID("pathfinder-open"_spr)) return true;

		auto sprite = CCSprite::createWithSpriteFrameName("GJ_optionsBtn_001.png");
		if (!sprite) sprite = ButtonSprite::create("PF");

		auto button = CCMenuItemSpriteExtra::create(
			sprite,
			this,
			menu_selector(PathfinderMenuLayer::onPathfinderButton)
		);
		button->setID("pathfinder-open"_spr);
		menu->addChild(button);
		menu->updateLayout();
		return true;
	}

	void onPathfinderButton(CCObject*) {
		PathfinderMenu::toggle();
	}
};
