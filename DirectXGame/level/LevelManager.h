#pragma once
#include "../scene/GameScene.h"
#include "../scene/SceneState.h"
#include "../scene/LoadingScene.h"
#include <vector>

class LevelManager {
public:
    LevelManager();
    ~LevelManager();

    void Initialize();
    void Update();
    void Draw();

    // 场景状态相关
    bool IsSceneEnd() const { return isSceneEnd_; }
    SceneState GetNextSceneState() const { return shouldReturnToTitle_ ? TITLE : RESULT; }

    // 新增：关卡切换状态
    enum class LevelState {
        Playing,    // 游戏中
        Loading,    // 加载中
        Transition  // 关卡切换中
    };

    LevelState GetCurrentState() const { return currentState_; }

    // 关卡控制
    void GoToNextLevel();
    void RestartCurrentLevel();
    void ReturnToTitle();

    // 获取当前关卡
    GameScene* GetCurrentLevel() const {
        return (currentLevelIndex_ < levels_.size()) ? levels_[currentLevelIndex_] : nullptr;
    }

private:
    std::vector<GameScene*> levels_;
    size_t currentLevelIndex_ = 0;
    bool shouldReturnToTitle_ = false;
    bool isSceneEnd_ = false;

    // 新增：Loading场景
    LoadingScene* loadingScene_ = nullptr;
    LevelState currentState_ = LevelState::Playing;

    void CreateLevels();
    void CleanupLevels();
    void StartLevelTransition();  // 开始关卡切换

    void UpdatePlayingState();
    void UpdateLoadingState();
    void UpdateTransitionState();
};