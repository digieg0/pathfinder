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

		return true;
	}
};
