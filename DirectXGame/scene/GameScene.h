#pragma once

#include "../play/Ball.h"
#include "../play/Goal.h"
#include "../scene/Stage.h"
#include "../play/GameUI.h"
#include "../play/GameLogicManager.h"
#include "../play/GoalMovementConfig.h"
#include "../play/IGameState.h"
#include "../play/IBallFactory.h"
#include "IScene.h"
#include "SceneState.h"
#include "KamataEngine.h"

#include <memory>
#include <vector>

using namespace KamataEngine;

/// <summary>
/// ゲームシーン
/// </summary>
class GameScene : public IScene {

public:
    /// <summary>
    /// GameScene に関する処理を行う。
    /// </summary>
    GameScene();
    /// <summary>
    /// ~GameScene に関する処理を行う。
    /// </summary>
    ~GameScene() override;

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

    bool IsSceneEnd() const override { return isSceneEnd_; }

    SceneState GetNextSceneState() const override {
        return returnToTitle_ ? TITLE : RESULT;
    }

    void SetLevelConfig(
        int levelNumber,
        const std::vector<KamataEngine::Vector3>& ballPositions,
        const std::vector<KamataEngine::Vector3>& goalPositions,
        const std::vector<int>& goalRequiredCounts = {},
        const std::vector<GoalMovementConfig>& goalMovementConfigs = {}
    );

    int GetLevelNumber() const { return levelNumber_; }

    // 状態変更
    /// <summary>
    /// 現在のゲーム状態を別の状態へ切り替える。
    /// </summary>
    void ChangeState(std::unique_ptr<IGameState> newState);

    // 供状态类调用的公共接口
    /// <summary>
    /// UpdateStageInternal に関する処理を行う。
    /// </summary>
    void UpdateStageInternal();
    /// <summary>
    /// UpdateUIInternal に関する処理を行う。
    /// </summary>
    void UpdateUIInternal();
    /// <summary>
    /// UpdateGameLogicInternal に関する処理を行う。
    /// </summary>
    void UpdateGameLogicInternal();
    /// <summary>
    /// UpdateBallsAndGoalsInternal に関する処理を行う。
    /// </summary>
    void UpdateBallsAndGoalsInternal();
    /// <summary>
    /// IsLevelComplete に関する処理を行う。
    /// </summary>
    bool IsLevelComplete() const;
    /// <summary>
    /// SetSceneEndFlag に関する処理を行う。
    /// </summary>
    void SetSceneEndFlag();

    // 供状态类调用的其他公共方法
    /// <summary>
    /// 現在のステージを初期状態に戻す。
    /// </summary>
    void RestartLevel();
    /// <summary>
    /// タイトル画面へ戻るための終了フラグを設定する。
    /// </summary>
    void ReturnToTitle();

    // 教程相关
    bool IsTutorialFinished() const {
        return currentTutorialIndex_ >= tutorialSprites_.size();
    }

    void ResetTutorial() {
        currentTutorialIndex_ = 0;
    }

    /// <summary>
    /// SetBallFactory に関する処理を行う。
    /// </summary>
    void SetBallFactory(std::unique_ptr<IBallFactory> factory);

private:
    DirectXCommon* dxCommon_ = nullptr;
    Input* input_ = nullptr;

    std::unique_ptr<Stage> stage_;
    std::unique_ptr<GameUI> gameUI_;
    GameLogicManager gameLogicManager_;
    std::unique_ptr<IBallFactory> ballFactory_;

    std::vector<std::unique_ptr<MyEngine::Ball>> balls_;
    std::vector<std::unique_ptr<MyEngine::Goal>> goals_;

    Camera camera_;

    /// <summary>
    /// チュートリアル、開始待機、開始演出の流れを表す列挙型。
    /// </summary>
    enum class GameFlowState {
        Tutorial,
        StartWait,
        StartAnim,
    };

    GameFlowState gameFlowState_ = GameFlowState::Tutorial;

    std::vector<uint32_t> tutorialTextureHandles_;
    std::vector<std::unique_ptr<KamataEngine::Sprite>> tutorialSprites_;

    uint32_t startTextureHandle_ = 0;
    std::unique_ptr<KamataEngine::Sprite> startSprite_;

    size_t currentTutorialIndex_ = 0;
    float startTimer_ = 0.0f;
    float animTimer_ = 0.0f;
    bool showStart_ = false;

    Vector2 startSize_ = {};
    Vector2 targetSize_ = {};

    float animDuration_ = 0.0f;
    float displayDuration_ = 0.0f;

    // 关卡数据
    int levelNumber_ = 1;
    std::vector<KamataEngine::Vector3> levelBallPositions_;
    std::vector<KamataEngine::Vector3> levelGoalPositions_;
    std::vector<int> levelGoalRequiredCounts_;
    std::vector<GoalMovementConfig> levelGoalMovementConfigs_;

    bool isSceneEnd_ = false;
    bool returnToTitle_ = false;

    std::unique_ptr<IGameState> currentState_;

private:
    /// <summary>
    /// LoadTutorialTextures に関する処理を行う。
    /// </summary>
    void LoadTutorialTextures();
    /// <summary>
    /// UpdateTutorial に関する処理を行う。
    /// </summary>
    void UpdateTutorial();
    /// <summary>
    /// UpdateStartWait に関する処理を行う。
    /// </summary>
    void UpdateStartWait();
    /// <summary>
    /// UpdateStartAnim に関する処理を行う。
    /// </summary>
    void UpdateStartAnim();
    /// <summary>
    /// StartGame に関する処理を行う。
    /// </summary>
    void StartGame();
    /// <summary>
    /// DrawTutorial に関する処理を行う。
    /// </summary>
    void DrawTutorial();

    /// <summary>
    /// InitializeLevelObjects に関する処理を行う。
    /// </summary>
    void InitializeLevelObjects();
    /// <summary>
    /// CheckLevelCompletion に関する処理を行う。
    /// </summary>
    void CheckLevelCompletion();
    /// <summary>
    /// ResetLevelCompletion に関する処理を行う。
    /// </summary>
    void ResetLevelCompletion();
    /// <summary>
    /// GameOver に関する処理を行う。
    /// </summary>
    void GameOver();
};
