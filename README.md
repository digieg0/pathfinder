# Pathfinder

A Geometry Dash Geode mod with an ImGui menu scaffold.

## Development

Open or close the Pathfinder ImGui window with either Alt key or the button on the Geometry Dash main menu. Use **Change shortcut** inside the window to assign another key; press Escape to cancel. **Reset to Alt** restores both Alt keys. The current window is a UI scaffold and does not change gameplay.

### Build

Install the Geode CLI and SDK, set `GEODE_SDK` to the SDK directory, then run:

```sh
geode build
```

CMake fetches the pinned ImGui/Cocos integration declared in `CMakeLists.txt`.

## Project layout

- `src/main.cpp` — Geode entry hooks and the main menu button.
- `src/ui/` — ImGui menu implementation.
- `cmake/` — CMake dependency bootstrap.
- `examples/` — local reference projects; excluded from this repository.
