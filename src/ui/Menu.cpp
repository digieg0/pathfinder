#include "Menu.hpp"
#include "../gameplay/RunStats.hpp"

#include <Geode/Geode.hpp>
#include <imgui-cocos.hpp>

namespace {
bool s_isOpen = false;
bool s_waitingForKey = false;

void installKeybindListeners() {
	geode::listenForKeybindSettingPresses("menu-toggle", [](geode::Keybind const&, bool down, bool repeat, double) {
		if (down && !repeat) s_isOpen = !s_isOpen;
	});

	geode::KeyboardInputEvent().listen([](geode::KeyboardInputData& data) {
		if (!s_waitingForKey || data.action != geode::KeyboardInputData::Action::Press) {
			return geode::ListenerResult::Propagate;
		}

		if (data.key == cocos2d::KEY_Escape) {
			s_waitingForKey = false;
			return geode::ListenerResult::Stop;
		}

		geode::Mod::get()->setSettingValue<std::vector<geode::Keybind>>(
			"menu-toggle",
			{ geode::Keybind(data.key, data.modifiers) }
		);
		s_waitingForKey = false;
		return geode::ListenerResult::Stop;
	}).leak();
}
}

void PathfinderMenu::install() {
	installKeybindListeners();
	ImGuiCocos::get().draw([] {
		if (!s_isOpen) return;

		ImGui::SetNextWindowSize(ImVec2(480.f, 360.f), ImGuiCond_FirstUseEver);
		if (ImGui::Begin("Pathfinder", &s_isOpen, ImGuiWindowFlags_NoCollapse)) {
			ImGui::TextUnformatted("Geometry Dash mod workspace");
			ImGui::Separator();
			ImGui::Spacing();
			ImGui::TextWrapped("The ImGui interface is ready. Mod features can be added here.");
			ImGui::Spacing();
			if (PathfinderRunStats::hasActiveAttempt()) {
				ImGui::Text("Current frame: %d", PathfinderRunStats::currentFrame());
			} else {
				ImGui::TextDisabled("Current frame: —");
			}
			if (PathfinderRunStats::hasPreviousDeath()) {
				ImGui::Text("Previous attempt death frame: %d", PathfinderRunStats::previousDeathFrame());
			} else {
				ImGui::TextDisabled("Previous attempt death frame: —");
			}
			if (ImGui::Button("Test jump")) {
				if (!PathfinderRunStats::testJump()) {
					geode::log::info("Test jump is available during an active, unpaused attempt.");
				}
			}
			ImGui::Spacing();

			auto keybinds = geode::Mod::get()->getSettingValue<std::vector<geode::Keybind>>("menu-toggle");
			std::string shortcut = keybinds.empty() ? "Unassigned" : keybinds.front().toString();
			ImGui::Text("Menu shortcut: %s", shortcut.c_str());
			if (s_waitingForKey) {
				ImGui::Button("Press a key... (Esc cancels)");
			} else if (ImGui::Button("Change shortcut")) {
				s_waitingForKey = true;
			}
			ImGui::SameLine();
			if (ImGui::Button("Reset to Alt")) {
				geode::Mod::get()->setSettingValue<std::vector<geode::Keybind>>(
					"menu-toggle",
					{ geode::Keybind(cocos2d::KEY_LeftMenu, geode::KeyboardModifier::None),
					  geode::Keybind(cocos2d::KEY_RightMenu, geode::KeyboardModifier::None) }
				);
				s_waitingForKey = false;
			}
			ImGui::Spacing();
			ImGui::TextDisabled("UI scaffold · no gameplay changes");
		}
		ImGui::End();
	});
}

void PathfinderMenu::toggle() {
	s_isOpen = !s_isOpen;
}
