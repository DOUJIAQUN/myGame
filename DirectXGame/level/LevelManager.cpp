#include "LevelManager.h"

#include <cassert>
#include <fstream>
#include <string>
#include <vector>

#include "../DebugLogger.h"
#include "../external/nlohmann/json.hpp"

using json = nlohmann::json;

LevelManager::LevelManager()
    : currentLevelIndex_(0),
    shouldReturnToTitle_(false),
    isSceneEnd_(false),
    currentState_(LevelState::Playing) {

    CreateLevels();

    loadingScene_ = std::make_unique<LoadingScene>();
    loadingScene_->Initialize();
}

LevelManager::~LevelManager() {
    CleanupLevels();

   
}

std::string LevelManager::GetCurrentLevelName() const {
    if (currentLevelIndex_ >= levelNames_.size()) {
        return "Unknown";
    }

    return levelNames_[currentLevelIndex_];
}

MoveDirection LevelManager::StringToMoveDirection(const std::string& directionText) const {//SR2_02_04
    if (directionText == "Horizontal") {
        return MoveDirection::Horizontal;
    }

    if (directionText == "Vertical") {
        return MoveDirection::Vertical;
    }

    if (directionText == "Circular") {
        return MoveDirection::Circular;
    }

    return MoveDirection::Horizontal;
}

std::vector<LevelManager::LevelConfig> LevelManager::LoadLevelConfigsFromJson(const std::string& filePath) const {//SR2_02_04
    std::vector<LevelConfig> configs;

    std::ifstream file(filePath);
    if (!file.is_open()) {
      
        return configs;
    }

    json root;
    file >> root;

    for (const auto& levelJson : root["levels"]) {
        LevelConfig config;

        config.levelNumber = levelJson["levelNumber"].get<int>();
        config.levelName = levelJson["levelName"].get<std::string>();

        for (const auto& ballJson : levelJson["balls"]) {
            KamataEngine::Vector3 position = {
                ballJson["x"].get<float>(),
                ballJson["y"].get<float>(),
                ballJson["z"].get<float>()
            };

            config.ballPositions.push_back(position);
        }

        for (const auto& goalJson : levelJson["goals"]) {
            const auto& positionJson = goalJson["position"];

            KamataEngine::Vector3 goalPosition = {
                positionJson["x"].get<float>(),
                positionJson["y"].get<float>(),
                positionJson["z"].get<float>()
            };

            config.goalPositions.push_back(goalPosition);

            int requiredCount = goalJson["requiredCount"].get<int>();
            config.goalRequiredCounts.push_back(requiredCount);

            const auto& movementJson = goalJson["movement"];

            bool shouldMove = movementJson["shouldMove"].get<bool>();
            std::string directionText = movementJson["direction"].get<std::string>();
            float range = movementJson["range"].get<float>();
            float speed = movementJson["speed"].get<float>();

            GoalMovementConfig movementConfig(
                shouldMove,
                StringToMoveDirection(directionText),
                range,
                speed
            );

            config.goalMovementConfigs.push_back(movementConfig);
        }

        configs.push_back(config);
    }

    return configs;
}

void LevelManager::CreateLevels() {
    CleanupLevels();
    levelNames_.clear();

    std::vector<LevelConfig> configs = LoadLevelConfigsFromJson("Resources/level_config.json");

    for (const LevelConfig& config : configs) {
        std::unique_ptr<GameScene> level = std::make_unique<GameScene>();

        level->SetLevelConfig(
            config.levelNumber,
            config.ballPositions,
            config.goalPositions,
            config.goalRequiredCounts,
            config.goalMovementConfigs
        );

        levels_.push_back(std::move(level));
        levelNames_.push_back(config.levelName);
    }
}

void LevelManager::SetCurrentLevel(int level) {
    if (level >= 1 && level <= static_cast<int>(levels_.size())) {
        currentLevelIndex_ = level - 1;
    }
}

void LevelManager::Initialize() {
    if (!levels_.empty()) {
        levels_[currentLevelIndex_]->Initialize();

        isSceneEnd_ = false;
        shouldReturnToTitle_ = false;
        currentState_ = LevelState::Playing;
    }
}

void LevelManager::Update() {
    if (isSceneEnd_) {
        return;
    }

    switch (currentState_) {
    case LevelState::Playing:
        UpdatePlayingState();
        break;

    case LevelState::Loading:
        UpdateLoadingState();
        break;

    case LevelState::Transition:
        UpdateTransitionState();
        break;

    case LevelState::GameComplete:
        UpdateGameCompleteState();
        break;
    }
}

void LevelManager::UpdatePlayingState() {
    if (currentLevelIndex_ >= levels_.size()) {
        return;
    }

    GameScene* currentLevel = levels_[currentLevelIndex_].get();
    if (!currentLevel) {
        return;
    }

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
            // 正常通关，开始加载下一关
            StartLevelTransition();
        }
    }
}

void LevelManager::UpdateLoadingState() {
    loadingScene_->Update();

    if (loadingScene_->IsSceneEnd()) {
        // 加载完成，进入下一关
        currentLevelIndex_++;

        if (currentLevelIndex_ < levels_.size()) {
            levels_[currentLevelIndex_]->Initialize();
            currentState_ = LevelState::Playing;
        }
        else {
            // 所有关卡完成
            currentState_ = LevelState::GameComplete;
            loadingScene_->StartLoading();
        }
    }
}

void LevelManager::UpdateTransitionState() {
    // 可以在这里添加其他过渡效果
    // 暂时为空
}

void LevelManager::UpdateGameCompleteState() {
    loadingScene_->Update();

    if (loadingScene_->IsSceneEnd()) {
        // 游戏完成Loading结束，设置场景结束
        isSceneEnd_ = true;
    }
}

void LevelManager::Draw() {
    switch (currentState_) {
    case LevelState::Playing:
        if (currentLevelIndex_ < levels_.size() && levels_[currentLevelIndex_]) {
            levels_[currentLevelIndex_]->Draw();
        }
        break;

    case LevelState::Loading:
    case LevelState::GameComplete:
        if (loadingScene_) {
            loadingScene_->Draw();
        }
        break;

    case LevelState::Transition:
        // 绘制过渡效果
        break;
    }
}

void LevelManager::StartLevelTransition() {
    if (currentLevelIndex_ + 1 < levels_.size()) {
        currentState_ = LevelState::Loading;
        loadingScene_->StartLoading();
    }
    else {
        // 已经是最后一关
        currentState_ = LevelState::GameComplete;
        loadingScene_->StartLoading();
    }
}

void LevelManager::GoToNextLevel() {
    currentLevelIndex_++;

    if (currentLevelIndex_ < levels_.size()) {
        // 初始化下一关
        levels_[currentLevelIndex_]->Initialize();
    }
    else {
        // 所有关卡完成
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

void LevelManager::CleanupLevels() {
   
    levels_.clear();
}