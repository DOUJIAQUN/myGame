#include "LevelManager.h"

#include <cassert>
#include <fstream>
#include <string>
#include <vector>

#include "../DebugLogger.h"
#include "../external/nlohmann/json.hpp"

using json = nlohmann::json;

namespace {
    const std::string kLevelConfigPath = "Resources/level_config.json";
}

LevelManager::LevelManager()
    : currentLevelIndex_(0),
    shouldReturnToTitle_(false),
    isSceneEnd_(false),
    // 関数コメント: currentState_ の処理を実行する。
    currentState_(LevelState::Playing) {
        // 処理コメント: 必要な状態確認やデータ更新を行い、currentState_ の役割を実現する。

    CreateLevels();

    loadingScene_ = std::make_unique<LoadingScene>();
    loadingScene_->Initialize();
}

// 関数コメント: ~LevelManager の処理を実行する。
LevelManager::~LevelManager() {
    // 処理コメント: 必要な状態確認やデータ更新を行い、~LevelManager の役割を実現する。
    CleanupLevels();

   
}

// 関数コメント: GetCurrentLevelName の処理を実行する。
std::string LevelManager::GetCurrentLevelName() const {
    // 処理コメント: 必要な状態確認やデータ更新を行い、GetCurrentLevelName の役割を実現する。
    if (currentLevelIndex_ >= levelNames_.size()) {
        return "Unknown";
    }

    return levelNames_[currentLevelIndex_];
}

// 関数コメント: StringToMoveDirection の処理を実行する。
MoveDirection LevelManager::StringToMoveDirection(const std::string& directionText) const {//SR2_02_04
    // 処理コメント: 必要な状態確認やデータ更新を行い、StringToMoveDirection の役割を実現する。
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

// 関数コメント: LoadLevelConfigsFromJson の処理を実行する。
std::vector<LevelManager::LevelConfig> LevelManager::LoadLevelConfigsFromJson(const std::string& filePath) const {//SR2_02_04
    // 処理コメント: 必要な状態確認やデータ更新を行い、LoadLevelConfigsFromJson の役割を実現する。
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

// 関数コメント: CreateLevels の処理を実行する。
void LevelManager::CreateLevels() {
    // 処理コメント: 必要な状態確認やデータ更新を行い、CreateLevels の役割を実現する。
    CleanupLevels();
    levelNames_.clear();

    std::vector<LevelConfig> configs = LoadLevelConfigsFromJson(kLevelConfigPath);

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

// 関数コメント: SetCurrentLevel の処理を実行する。
void LevelManager::SetCurrentLevel(int level) {
    // 処理コメント: 必要な状態確認やデータ更新を行い、SetCurrentLevel の役割を実現する。
    if (level >= 1 && level <= static_cast<int>(levels_.size())) {
        currentLevelIndex_ = level - 1;
    }
}

// 関数コメント: Initialize の処理を実行する。
void LevelManager::Initialize() {
    // 処理コメント: 必要な状態確認やデータ更新を行い、Initialize の役割を実現する。
    if (!levels_.empty()) {
        levels_[currentLevelIndex_]->Initialize();

        isSceneEnd_ = false;
        shouldReturnToTitle_ = false;
        currentState_ = LevelState::Playing;
    }
}

// 関数コメント: Update の処理を実行する。
void LevelManager::Update() {
    // 処理コメント: 必要な状態確認やデータ更新を行い、Update の役割を実現する。
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

// 関数コメント: UpdatePlayingState の処理を実行する。
void LevelManager::UpdatePlayingState() {
    // 処理コメント: 必要な状態確認やデータ更新を行い、UpdatePlayingState の役割を実現する。
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

// 関数コメント: UpdateLoadingState の処理を実行する。
void LevelManager::UpdateLoadingState() {
    // 処理コメント: 必要な状態確認やデータ更新を行い、UpdateLoadingState の役割を実現する。
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

// 関数コメント: UpdateTransitionState の処理を実行する。
void LevelManager::UpdateTransitionState() {
    // 処理コメント: 必要な状態確認やデータ更新を行い、UpdateTransitionState の役割を実現する。
    // 可以在这里添加其他过渡效果
    // 暂时为空
}

// 関数コメント: UpdateGameCompleteState の処理を実行する。
void LevelManager::UpdateGameCompleteState() {
    // 処理コメント: 必要な状態確認やデータ更新を行い、UpdateGameCompleteState の役割を実現する。
    loadingScene_->Update();

    if (loadingScene_->IsSceneEnd()) {
        // 游戏完成Loading结束，设置场景结束
        isSceneEnd_ = true;
    }
}

// 関数コメント: Draw の処理を実行する。
void LevelManager::Draw() {
    // 処理コメント: 必要な状態確認やデータ更新を行い、Draw の役割を実現する。
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

// 関数コメント: StartLevelTransition の処理を実行する。
void LevelManager::StartLevelTransition() {
    // 処理コメント: 必要な状態確認やデータ更新を行い、StartLevelTransition の役割を実現する。
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

// 関数コメント: GoToNextLevel の処理を実行する。
void LevelManager::GoToNextLevel() {
    // 処理コメント: 必要な状態確認やデータ更新を行い、GoToNextLevel の役割を実現する。
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

// 関数コメント: RestartCurrentLevel の処理を実行する。
void LevelManager::RestartCurrentLevel() {
    // 処理コメント: 必要な状態確認やデータ更新を行い、RestartCurrentLevel の役割を実現する。
    if (currentLevelIndex_ < levels_.size() && levels_[currentLevelIndex_]) {
        levels_[currentLevelIndex_]->Initialize();
    }
}

// 関数コメント: ReturnToTitle の処理を実行する。
void LevelManager::ReturnToTitle() {
    // 処理コメント: 必要な状態確認やデータ更新を行い、ReturnToTitle の役割を実現する。
    shouldReturnToTitle_ = true;
    isSceneEnd_ = true;
}

// 関数コメント: CleanupLevels の処理を実行する。
void LevelManager::CleanupLevels() {
    // 処理コメント: 必要な状態確認やデータ更新を行い、CleanupLevels の役割を実現する。
   
    levels_.clear();
}
