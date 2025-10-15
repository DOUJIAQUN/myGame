#include "LevelManager.h"

LevelManager::LevelManager() { currentLevelNumber_ = 1; }

LevelManager::~LevelManager() {
	// unique_ptr会自动清理
}

void LevelManager::Initialize() { LoadLevel(1); }

void LevelManager::Update() {
	if (currentLevel_) {
		currentLevel_->Update();
	}
}

void LevelManager::Draw() {
	if (currentLevel_) {
		currentLevel_->Draw();
	}
}

void LevelManager::LoadLevel(int levelNumber) {
	currentLevel_ = CreateLevel(levelNumber);
	if (currentLevel_) {
		currentLevel_->Initialize();
		currentLevel_->OnEnter();
		currentLevelNumber_ = levelNumber; // 更新关卡编号
	}
}

void LevelManager::NextLevel() {
	if (currentLevelNumber_ < totalLevels_) {
		LoadLevel(currentLevelNumber_ + 1);
	}
}

void LevelManager::RestartLevel() { LoadLevel(currentLevelNumber_); }

LevelBase::LevelState LevelManager::GetLevelState() const { return currentLevel_ ? currentLevel_->GetLevelState() : LevelBase::LevelState::FAILED; }

bool LevelManager::IsLevelComplete() const { return currentLevel_ && currentLevel_->IsComplete(); }

const char* LevelManager::GetCurrentLevelName() const { return currentLevel_ ? currentLevel_->GetLevelName() : "Unknown Level"; }

std::unique_ptr<LevelBase> LevelManager::CreateLevel(int levelNumber) {
	switch (levelNumber) {
	case 1:
		return std::make_unique<Level1>();

	default:
		return std::make_unique<Level1>(); // 默认返回第一关
	}
}