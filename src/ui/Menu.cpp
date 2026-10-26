#include "Menu.hpp"
#include "../gameplay/RunStats.hpp"
#include "../diagnostics/Logger.hpp"

#include <Geode/Geode.hpp>
#include <imgui-cocos.hpp>

namespace {
bool s_isOpen = false;
bool s_waitingForKey = false;

void installKeybindListeners() {
	geode::KeyboardInputEvent().listen([](geode::KeyboardInputData& data) {
		if (data.action != geode::KeyboardInputData::Action::Press &&
			data.action != geode::KeyboardInputData::Action::Repeat) {
			return geode::ListenerResult::Propagate;
		}
		if (data.action == geode::KeyboardInputData::Action::Press) {
			PathfinderDiagnostics::write(fmt::format(
				"keyboard press: code={} modifiers={}",
				static_cast<int>(data.key),
				static_cast<int>(data.modifiers)
			));
		}
		if (s_waitingForKey) {
			if (data.action == geode::KeyboardInputData::Action::Repeat) {
				return geode::ListenerResult::Stop;
			}
			if (data.key == cocos2d::KEY_Escape) {
				s_waitingForKey = false;
				PathfinderDiagnostics::write("shortcut capture cancelled with Escape");
				return geode::ListenerResult::Stop;
			}

			geode::Mod::get()->setSettingValue<std::vector<geode::Keybind>>(
				"menu-toggle",
				{ geode::Keybind(data.key, data.modifiers) }
			);
			s_waitingForKey = false;
			PathfinderDiagnostics::write(fmt::format(
				"shortcut captured: {} key={} modifiers={}",
				geode::Keybind(data.key, data.modifiers).toString(),
				static_cast<int>(data.key),
				static_cast<int>(data.modifiers)
			));
			return geode::ListenerResult::Stop;
		}

		if (data.action == geode::KeyboardInputData::Action::Press) {
			geode::Keybind const pressed(data.key, data.modifiers);
			for (auto const& bind : geode::Mod::get()->getSettingValue<std::vector<geode::Keybind>>("menu-toggle")) {
				bool legacyAltBind =
					(pressed.key == cocos2d::KEY_LeftMenu || pressed.key == cocos2d::KEY_RightMenu) &&
					pressed.modifiers == geode::KeyboardModifier::Alt &&
					bind.key == pressed.key &&
					bind.modifiers == geode::KeyboardModifier::None;
				if (bind == pressed || legacyAltBind) {
					PathfinderDiagnostics::write(fmt::format(
						"shortcut matched: {} key={} modifiers={}; menu {} -> {}",
						bind.toString(), static_cast<int>(bind.key), static_cast<int>(bind.modifiers),
						s_isOpen ? "open" : "closed", s_isOpen ? "closed" : "open"
					));
					s_isOpen = !s_isOpen;
					return geode::ListenerResult::Stop;
				}
			}
		}
		return geode::ListenerResult::Propagate;
	}).leak();
}
}

void PathfinderMenu::install() {
	PathfinderDiagnostics::write("PathfinderMenu::install entered");
	installKeybindListeners();
	PathfinderDiagnostics::write("KeyboardInputEvent listener installed");
	ImGuiCocos::get().setup([] {
		PathfinderDiagnostics::write("ImGuiCocos setup callback entered");
		ImGui::GetIO().IniFilename = nullptr;
	}).draw([] {
		static bool firstDraw = true;
		if (firstDraw) {
			PathfinderDiagnostics::write("ImGuiCocos draw callback entered");
			firstDraw = false;
		}
		if (!s_isOpen) return;

		ImGui::SetNextWindowSize(ImVec2(480.f, 360.f), ImGuiCond_FirstUseEver);
		bool wasOpen = s_isOpen;
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
				PathfinderDiagnostics::write("shortcut capture started from menu");
			}
			ImGui::SameLine();
			if (ImGui::Button("Reset to Alt")) {
				geode::Mod::get()->setSettingValue<std::vector<geode::Keybind>>(
					"menu-toggle",
					{ geode::Keybind(cocos2d::KEY_LeftMenu, geode::KeyboardModifier::Alt),
					  geode::Keybind(cocos2d::KEY_RightMenu, geode::KeyboardModifier::Alt) }
				);
				s_waitingForKey = false;
			}
			ImGui::Spacing();
			ImGui::TextDisabled("UI scaffold · no gameplay changes");
		}
		ImGui::End();
		if (wasOpen && !s_isOpen) {
			PathfinderDiagnostics::write("ImGui window closed with title bar X");
		}
	});
	PathfinderDiagnostics::write("ImGuiCocos setup/draw callbacks registered");
}

void PathfinderMenu::toggle() {
	s_isOpen = !s_isOpen;
}
