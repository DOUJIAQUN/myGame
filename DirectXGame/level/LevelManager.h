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

    //关卡切换状态
    enum class LevelState {
        Playing,    // 游戏中
        Loading,    // 加载中
        Transition,  // 关卡切换中
        GameComplete //游戏完成，准备显示结果前的Loading
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
    //设置当前关卡
    void SetCurrentLevel(int level);

    //获取关卡信息
    int GetCurrentLevelNumber() const {
        return (currentLevelIndex_ < levels_.size()) ? levels_[currentLevelIndex_]->GetLevelNumber() : 0;
    }
    std::string GetCurrentLevelName() const;


private:
    std::vector<GameScene*> levels_;
    size_t currentLevelIndex_ = 0;
    bool shouldReturnToTitle_ = false;
    bool isSceneEnd_ = false;

    //Loading场景
    LoadingScene* loadingScene_ = nullptr;
    LevelState currentState_ = LevelState::Playing;

    void CreateLevels();
    void CleanupLevels();
    void StartLevelTransition();  // 开始关卡切换

    void UpdatePlayingState();
    void UpdateLoadingState();
    void UpdateTransitionState();
    void UpdateGameCompleteState();
};