#pragma once
#include "Ball.h"
#include "KamataEngine.h"
#include <vector>
using namespace KamataEngine;

// 前向声明
class Ball;

class LevelBase {
public:
	enum class LevelState { PLAYING, COMPLETED, FAILED };

	virtual ~LevelBase();

	virtual void Initialize() = 0;
	virtual void Update() = 0;
	virtual void Draw() = 0;

	virtual void OnEnter() {}
	virtual void OnExit() {}

	LevelState GetLevelState() const { return levelState_; }
	int GetLevelNumber() const { return levelNumber_; }
	bool IsComplete() const { return levelState_ == LevelState::COMPLETED; }

	virtual const char* GetLevelName() const = 0;

protected:
	std::vector<Ball*> balls_;
	Camera camera_;
	LevelState levelState_ = LevelState::PLAYING;
	int levelNumber_ = 0;

	void ClearBalls();
	void TriggerExplosion(const KamataEngine::Vector3& position, float radius = 10.0f, float force = 10.0f);
};