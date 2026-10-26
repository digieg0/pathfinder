# Pathfinder

A Geometry Dash Geode mod with an ImGui menu scaffold.

## Development

Open or close the Pathfinder ImGui window with either Alt key or the button on the Geometry Dash main menu. Use **Change shortcut** inside the window to assign another key; press Escape to cancel. **Reset to Alt** restores both Alt keys. During a level, the window shows the current attempt frame and the previous attempt's death frame. **Test jump** sends a jump input to the player. The rest of the window is a UI scaffold.

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
