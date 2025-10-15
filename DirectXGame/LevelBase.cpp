#include "LevelBase.h"
#include "myMath.h"
#include <cassert>

LevelBase::~LevelBase() { ClearBalls(); }

void LevelBase::ClearBalls() {
	for (Ball* ball : balls_) {
		delete ball;
	}
	balls_.clear();
}

void LevelBase::TriggerExplosion(const KamataEngine::Vector3& position, float radius, float force) {
	// 对范围内的球体施加爆炸力
	for (Ball* ball : balls_) {
		if (ball->IsActive()) {
			float distance = myMath::Distance(position, ball->GetPosition());

			if (distance <= radius) {
				KamataEngine::Vector3 direction = myMath::Subtract(ball->GetPosition(), position);
				direction = myMath::Normalize(direction);

				float forceStrength = force * (1.0f - distance / radius);
				KamataEngine::Vector3 explosionForce = myMath::Multiply(forceStrength, direction);

				ball->ApplyExplosionForce(explosionForce);
			}
		}
	}
}