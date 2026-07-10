#pragma once

#include "../scene/GameScene.h"
#include "../scene/SceneState.h"
#include "../scene/LoadingScene.h"
#include "../scene/IScene.h"

#include <vector>
#include <string>
#include <memory>

/// <summary>
/// 複数の GameScene を保持し、ステージ進行とシーン遷移を管理するクラス。
/// </summary>
class LevelManager : public IScene {
public:
    /// <summary>
    /// LevelManager に関する処理を行う。
    /// </summary>
    LevelManager();
    /// <summary>
    /// ~LevelManager に関する処理を行う。
    /// </summary>
    ~LevelManager();

    /// <summary>
    /// オブジェクトやシーンの初期化処理を行う。
    /// </summary>
    void Initialize() override;
    /// <summary>
    /// 毎フレームの更新処理を行う。
    /// </summary>
    void Update() override;
    /// <summary>
    /// 描画処理を行う。
    /// </summary>
    void Draw() override;

    // 场景状态相关
    bool IsSceneEnd() const override { return isSceneEnd_; }
    SceneState GetNextSceneState() const override { return shouldReturnToTitle_ ? TITLE : RESULT; }

    // 关卡切换状态
    /// <summary>
    /// LevelManager 内でのプレイ中、ロード中、遷移中、全クリア状態を表す列挙型。
    /// </summary>
    enum class LevelState {
        Playing,      // 游戏中
        Loading,      // 加载中
        Transition,   // 关卡切换中
        GameComplete  // 游戏完成，准备显示结果前的Loading
    };

    LevelState GetCurrentState() const { return currentState_; }

    // 关卡控制
    /// <summary>
    /// GoToNextLevel に関する処理を行う。
    /// </summary>
    void GoToNextLevel();
    /// <summary>
    /// RestartCurrentLevel に関する処理を行う。
    /// </summary>
    void RestartCurrentLevel();
    /// <summary>
    /// タイトル画面へ戻るための終了フラグを設定する。
    /// </summary>
    void ReturnToTitle();

    // 获取当前关卡
    GameScene* GetCurrentLevel() const {
        return (currentLevelIndex_ < levels_.size()) ? levels_[currentLevelIndex_].get() : nullptr;
    }

    // 设置当前关卡
    /// <summary>
    /// SetCurrentLevel に関する処理を行う。
    /// </summary>
    void SetCurrentLevel(int level);

    // 获取关卡信息
    int GetCurrentLevelNumber() const {
        return (currentLevelIndex_ < levels_.size()) ? levels_[currentLevelIndex_]->GetLevelNumber() : 0;
    }

    /// <summary>
    /// GetCurrentLevelName に関する処理を行う。
    /// </summary>
    std::string GetCurrentLevelName() const;

private:
    // 关卡数据
    // 各关卡之间不同的数值集中放在这里，CreateLevels() 只负责统一生成
    /// <summary>
    /// 外部 JSON から読み込んだ 1 ステージ分の設定情報を保持する構造体。
    /// </summary>
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
    /// <summary>
    /// CreateLevels に関する処理を行う。
    /// </summary>
    void CreateLevels();

    /// <summary>
    /// LoadLevelConfigsFromJson に関する処理を行う。
    /// </summary>
    std::vector<LevelConfig> LoadLevelConfigsFromJson(const std::string& filePath) const;
    /// <summary>
    /// StringToMoveDirection に関する処理を行う。
    /// </summary>
    MoveDirection StringToMoveDirection(const std::string& directionText) const;
    
    /// <summary>
    /// CleanupLevels に関する処理を行う。
    /// </summary>
    void CleanupLevels();
    /// <summary>
    /// StartLevelTransition に関する処理を行う。
    /// </summary>
    void StartLevelTransition();

    /// <summary>
    /// UpdatePlayingState に関する処理を行う。
    /// </summary>
    void UpdatePlayingState();
    /// <summary>
    /// UpdateLoadingState に関する処理を行う。
    /// </summary>
    void UpdateLoadingState();
    /// <summary>
    /// UpdateTransitionState に関する処理を行う。
    /// </summary>
    void UpdateTransitionState();
    /// <summary>
    /// UpdateGameCompleteState に関する処理を行う。
    /// </summary>
    void UpdateGameCompleteState();
};
