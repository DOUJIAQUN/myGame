#include "../level/LevelManager.h"
#include <cassert>

LevelManager::LevelManager()
    : currentLevelIndex_(0),
    shouldReturnToTitle_(false),
    isSceneEnd_(false) {
    CreateLevels();
}

LevelManager::~LevelManager() {
    CleanupLevels();
}

void LevelManager::Initialize() {
    if (!levels_.empty()) {
        levels_[0]->Initialize();
        isSceneEnd_ = false;
        shouldReturnToTitle_ = false;
    }
}

void LevelManager::Update() {
    if (isSceneEnd_ || currentLevelIndex_ >= levels_.size()) return;

    GameScene* currentLevel = levels_[currentLevelIndex_];
    if (!currentLevel) return;

    currentLevel->Update();

    // 检查当前关卡是否结束
    if (currentLevel->IsSceneEnd()) {
        SceneState nextState = currentLevel->GetNextSceneState();

        if (nextState == TITLE) {
            // 返回标题
            shouldReturnToTitle_ = true;
            isSceneEnd_ = true;
        }
        else {
            // 正常通关，进入下一关
            GoToNextLevel();
        }
    }
}

void LevelManager::Draw() {
    if (currentLevelIndex_ < levels_.size() && levels_[currentLevelIndex_]) {
        levels_[currentLevelIndex_]->Draw();
    }
}

void LevelManager::GoToNextLevel() {
    currentLevelIndex_++;

    if (currentLevelIndex_ < levels_.size()) {
        // 初始化下一关
        printf("进入第 %zu 关\n", currentLevelIndex_ + 1);
        levels_[currentLevelIndex_]->Initialize();
    }
    else {
        // 所有关卡完成
        printf("所有关卡完成！\n");
        isSceneEnd_ = true;
    }
}

void LevelManager::RestartCurrentLevel() {
    if (currentLevelIndex_ < levels_.size() && levels_[currentLevelIndex_]) {
        levels_[currentLevelIndex_]->Initialize();
    }
}

void LevelManager::ReturnToTitle() {
    shouldReturnToTitle_ = true;
    isSceneEnd_ = true;
}

void LevelManager::CreateLevels() {
    // 创建第一关
    GameScene* level1 = new GameScene();
    levels_.push_back(level1);

    // 创建第二关
    GameScene* level2 = new GameScene();
    levels_.push_back(level2);

    printf("创建了 %zu 个关卡\n", levels_.size());
}

void LevelManager::CleanupLevels() {
    for (auto level : levels_) {
        if (level) {
            delete level;
        }
    }
    levels_.clear();
}