#pragma once

#include <string_view>

namespace PathfinderDiagnostics {
void start();
void write(std::string_view message);
} // namespace PathfinderDiagnostics
