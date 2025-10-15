#pragma once
#include "LevelBase.h"

class Level1 : public LevelBase {
public:
	Level1() { levelNumber_ = 1; }

	void Initialize() override;
	void Update() override;
	void Draw() override;
	void OnEnter() override;

	const char* GetLevelName() const override { return "Tutorial - Basic Explosion"; }

private:
	bool explosionTriggered_ = false;
	KamataEngine::Input* input_ = nullptr;
};