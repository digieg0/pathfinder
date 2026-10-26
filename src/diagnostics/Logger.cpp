#include "Logger.hpp"

#include <Geode/Geode.hpp>

#include <chrono>
#include <ctime>
#include <filesystem>
#include <fstream>
#include <iomanip>
#include <mutex>
#include <sstream>

namespace {
const std::filesystem::path s_logPath = "D:/Project/ai_gd/logs/pathfinder.log";
std::mutex s_logMutex;

std::string timestamp() {
	auto now = std::chrono::system_clock::now();
	auto time = std::chrono::system_clock::to_time_t(now);
	std::tm localTime{};
	localtime_s(&localTime, &time);

	std::ostringstream out;
	out << std::put_time(&localTime, "%Y-%m-%d %H:%M:%S");
	return out.str();
}
}

void PathfinderDiagnostics::start() {
	std::lock_guard lock(s_logMutex);
	std::error_code error;
	std::filesystem::create_directories(s_logPath.parent_path(), error);
	std::ofstream file(s_logPath, std::ios::trunc);
	if (!file) {
		geode::log::error("Pathfinder could not open diagnostic log at {}", s_logPath.string());
		return;
	}
	file << timestamp() << " session started\n";
	file << timestamp() << " log path: " << s_logPath.string() << "\n";
	file.flush();
}

void PathfinderDiagnostics::write(std::string_view message) {
	std::lock_guard lock(s_logMutex);
	std::error_code error;
	std::filesystem::create_directories(s_logPath.parent_path(), error);
	std::ofstream file(s_logPath, std::ios::app);
	if (!file) {
		geode::log::error("Pathfinder could not append to diagnostic log at {}", s_logPath.string());
		return;
	}
	file << timestamp() << ' ' << message << '\n';
	file.flush();
}
