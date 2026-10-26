#include "Menu.hpp"

#include <Geode/Geode.hpp>
#include <imgui-cocos.hpp>

namespace {
bool s_isOpen = false;
}

void PathfinderMenu::install() {
	ImGuiCocos::get().draw([] {
		if (!s_isOpen) return;

		ImGui::SetNextWindowSize(ImVec2(430.f, 250.f), ImGuiCond_FirstUseEver);
		if (ImGui::Begin("Pathfinder", &s_isOpen, ImGuiWindowFlags_NoCollapse)) {
			ImGui::TextUnformatted("Geometry Dash mod workspace");
			ImGui::Separator();
			ImGui::Spacing();
			ImGui::TextWrapped("The ImGui interface is ready. Mod features can be added here.");
			ImGui::Spacing();
			ImGui::TextDisabled("UI scaffold · no gameplay changes");
		}
		ImGui::End();
	});
}

void PathfinderMenu::toggle() {
	s_isOpen = !s_isOpen;
}
