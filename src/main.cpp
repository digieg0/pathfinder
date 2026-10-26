#include <Geode/Geode.hpp>
#include <Geode/modify/MenuLayer.hpp>

#include "diagnostics/Logger.hpp"
#include "ui/Menu.hpp"

using namespace geode::prelude;

$on_mod(Loaded) {
	PathfinderDiagnostics::start();
	PathfinderDiagnostics::write("Geode reported Pathfinder mod loaded");
	auto keybinds = Mod::get()->getSettingValue<std::vector<geode::Keybind>>("menu-toggle");
	for (auto const& bind : keybinds) {
		PathfinderDiagnostics::write(fmt::format(
			"loaded menu binding: {} key={} modifiers={}",
			bind.toString(), static_cast<int>(bind.key), static_cast<int>(bind.modifiers)
		));
	}
}

class $modify(PathfinderMenuLayer, MenuLayer) {
	bool init() {
		if (!MenuLayer::init()) return false;
		PathfinderDiagnostics::write("MenuLayer::init completed");

		static bool imguiReady = false;
		if (!imguiReady) {
			PathfinderDiagnostics::write("installing Pathfinder menu and ImGui callbacks");
			PathfinderMenu::install();
			imguiReady = true;
		}

		return true;
	}
};
