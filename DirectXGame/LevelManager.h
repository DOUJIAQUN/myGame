#pragma once
#include "Level1.h"

#include "LevelBase.h"
#include <memory>
#include <vector>

class LevelManager {
public:
	// 将LevelState枚举移到LevelManager内部或使用LevelBase的枚举
	using LevelState = LevelBase::LevelState;

	LevelManager();
	~LevelManager();

	void Initialize();
	void Update();
	void Draw();

	void LoadLevel(int levelNumber);
	void NextLevel();
	void RestartLevel();

	LevelState GetLevelState() const;
	int GetCurrentLevelNumber() const { return currentLevelNumber_; }
	int GetTotalLevels() const { return totalLevels_; }
	bool IsLevelComplete() const;
	bool IsGameComplete() const { return currentLevelNumber_ > totalLevels_; }

	const char* GetCurrentLevelName() const;

private:
	std::unique_ptr<LevelBase> currentLevel_;
	int currentLevelNumber_ = 1;
	int totalLevels_ = 1;

	std::unique_ptr<LevelBase> CreateLevel(int levelNumber);
};