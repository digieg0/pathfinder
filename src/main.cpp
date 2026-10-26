#include <Geode/Geode.hpp>
#include <Geode/modify/CCKeyboardDispatcher.hpp>
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

class $modify(PathfinderKeyboardDispatcher, CCKeyboardDispatcher) {
	bool dispatchKeyboardMSG(enumKeyCodes key, bool isKeyDown, bool isKeyRepeat, double timestamp) {
		if (isKeyDown && !isKeyRepeat) {
			geode::KeyboardModifier modifiers = geode::KeyboardModifier::None;
			if (this->getAltKeyPressed()) modifiers |= geode::KeyboardModifier::Alt;
			if (this->getControlKeyPressed()) modifiers |= geode::KeyboardModifier::Control;
			if (this->getShiftKeyPressed()) modifiers |= geode::KeyboardModifier::Shift;
			if (this->getCommandKeyPressed()) modifiers |= geode::KeyboardModifier::Super;

			switch (key) {
				case KEY_LeftMenu:
			case KEY_RightMenu:
				modifiers &= ~geode::KeyboardModifier::Alt;
				break;
				case KEY_LeftShift:
				case KEY_RightShift:
				modifiers &= ~geode::KeyboardModifier::Shift;
				break;
				case KEY_LeftControl:
				case KEY_RightContol:
				modifiers &= ~geode::KeyboardModifier::Control;
				break;
				case KEY_LeftWindowsKey:
				case KEY_RightWindowsKey:
				modifiers &= ~geode::KeyboardModifier::Super;
				break;
				default:
				break;
			}

			for (auto const& keybind : Mod::get()->getSettingValue<std::vector<geode::Keybind>>("menu-toggle")) {
				auto bindingModifiers = keybind.modifiers;
				if (keybind.key == KEY_LeftMenu || keybind.key == KEY_RightMenu) {
					bindingModifiers &= ~geode::KeyboardModifier::Alt;
				} else if (keybind.key == KEY_LeftShift || keybind.key == KEY_RightShift) {
					bindingModifiers &= ~geode::KeyboardModifier::Shift;
				} else if (keybind.key == KEY_LeftControl || keybind.key == KEY_RightContol) {
					bindingModifiers &= ~geode::KeyboardModifier::Control;
				} else if (keybind.key == KEY_LeftWindowsKey || keybind.key == KEY_RightWindowsKey) {
					bindingModifiers &= ~geode::KeyboardModifier::Super;
				}

				if (keybind.key == key && bindingModifiers == modifiers) {
					PathfinderMenu::toggle();
					return true;
				}
			}
		}

		return CCKeyboardDispatcher::dispatchKeyboardMSG(key, isKeyDown, isKeyRepeat, timestamp);
	}
};
