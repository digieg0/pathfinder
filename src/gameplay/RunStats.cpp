#include "RunStats.hpp"

#include <Geode/Geode.hpp>
#include <Geode/modify/PlayLayer.hpp>

using namespace geode::prelude;

namespace {
int s_currentFrame = 0;
int s_previousDeathFrame = 0;
bool s_hasPreviousDeath = false;
bool s_deathRecordedThisAttempt = false;
}

class $modify(PathfinderPlayLayer, PlayLayer) {
	bool init(GJGameLevel* level, bool useReplay, bool dontCreateObjects) {
		s_currentFrame = 0;
		s_previousDeathFrame = 0;
		s_hasPreviousDeath = false;
		s_deathRecordedThisAttempt = false;
		return PlayLayer::init(level, useReplay, dontCreateObjects);
	}

	void postUpdate(float dt) {
		PlayLayer::postUpdate(dt);
		if (m_player1 && !m_player1->m_isDead) ++s_currentFrame;
	}

	void resetLevel() {
		PlayLayer::resetLevel();
		s_currentFrame = 0;
		s_deathRecordedThisAttempt = false;
	}

	void destroyPlayer(PlayerObject* player, GameObject* object) {
		if (player == m_player1 && !s_deathRecordedThisAttempt) {
			s_previousDeathFrame = s_currentFrame;
			s_hasPreviousDeath = true;
			s_deathRecordedThisAttempt = true;
		}
		PlayLayer::destroyPlayer(player, object);
	}
};

bool PathfinderRunStats::hasActiveAttempt() {
	return PlayLayer::get() != nullptr;
}

int PathfinderRunStats::currentFrame() {
	return s_currentFrame;
}

int PathfinderRunStats::previousDeathFrame() {
	return s_previousDeathFrame;
}

bool PathfinderRunStats::hasPreviousDeath() {
	return s_hasPreviousDeath;
}

bool PathfinderRunStats::testJump() {
	auto layer = PlayLayer::get();
	if (!layer || !layer->m_player1 || layer->m_player1->m_isDead || layer->m_isPaused) {
		return false;
	}

	layer->m_player1->pushButton(PlayerButton::Jump);
	layer->m_player1->releaseButton(PlayerButton::Jump);
	return true;
}
