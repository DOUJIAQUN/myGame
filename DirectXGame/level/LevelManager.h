#pragma once

#include "../scene/GameScene.h"
#include "../scene/SceneState.h"
#include "../scene/LoadingScene.h"
#include "../scene/IScene.h"

#include <vector>
#include <string>
#include <memory>

class LevelManager : public IScene {
public:
    LevelManager();
    ~LevelManager();

    void Initialize() override;
    void Update() override;
    void Draw() override;

    // 场景状态相关
    bool IsSceneEnd() const override { return isSceneEnd_; }
    SceneState GetNextSceneState() const override { return shouldReturnToTitle_ ? TITLE : RESULT; }

    // 关卡切换状态
    enum class LevelState {
        Playing,      // 游戏中
        Loading,      // 加载中
        Transition,   // 关卡切换中
        GameComplete  // 游戏完成，准备显示结果前的Loading
    };

    LevelState GetCurrentState() const { return currentState_; }

    // 关卡控制
    void GoToNextLevel();
    void RestartCurrentLevel();
    void ReturnToTitle();

    // 获取当前关卡
    GameScene* GetCurrentLevel() const {
        return (currentLevelIndex_ < levels_.size()) ? levels_[currentLevelIndex_].get() : nullptr;
    }

    // 设置当前关卡
    void SetCurrentLevel(int level);

    // 获取关卡信息
    int GetCurrentLevelNumber() const {
        return (currentLevelIndex_ < levels_.size()) ? levels_[currentLevelIndex_]->GetLevelNumber() : 0;
    }

    std::string GetCurrentLevelName() const;

private:
    // 关卡数据
    // 各关卡之间不同的数值集中放在这里，CreateLevels() 只负责统一生成
    struct LevelConfig {
        int levelNumber;
        std::string levelName;

        std::vector<KamataEngine::Vector3> ballPositions;
        std::vector<KamataEngine::Vector3> goalPositions;
        std::vector<int> goalRequiredCounts;
        std::vector<GoalMovementConfig> goalMovementConfigs;
    };

private:
    std::vector<std::unique_ptr<GameScene>> levels_;
    std::vector<std::string> levelNames_;

    size_t currentLevelIndex_ = 0;

    bool shouldReturnToTitle_ = false;
    bool isSceneEnd_ = false;

    // Loading场景
    std::unique_ptr<LoadingScene> loadingScene_;
    LevelState currentState_ = LevelState::Playing;

private:
    void CreateLevels();

    std::vector<LevelConfig> LoadLevelConfigsFromJson(const std::string& filePath) const;
    MoveDirection StringToMoveDirection(const std::string& directionText) const;
    
    void CleanupLevels();
    void StartLevelTransition();

    void UpdatePlayingState();
    void UpdateLoadingState();
    void UpdateTransitionState();
    void UpdateGameCompleteState();
};