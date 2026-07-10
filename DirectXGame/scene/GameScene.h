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

namespace MyEngine {

/// <summary>
/// ゲームシーン
/// </summary>
class GameScene : public IScene {

public:
    /// <summary>
/// ゲーム本編の Stage、Ball、Goal、UI、状態遷移を統合して管理する。
/// </summary>
    GameScene();
    /// <summary>
/// GameScene が所有する Stage、UI、Ball、Goal、State を unique_ptr により解放する。
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
/// LevelManager から受け取ったステージ番号、Ball 配置、Goal 設定を GameScene に保存する。
/// </summary>
/// <param name="levelNumber">ステージ番号。</param>
/// <param name="ballPositions">生成する Ball の初期座標一覧。</param>
/// <param name="goalPositions">生成する Goal の初期座標一覧。</param>
/// <param name="goalRequiredCounts">各 Goal に必要な到達回数。</param>
/// <param name="goalMovementConfigs">各 Goal の移動設定。</param>
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
/// 現在の State を終了させ、次の State に所有権を移して Enter を呼び出す。
/// </summary>
/// <param name="newState">切り替え先のゲーム状態。unique_ptr で所有権を受け取る。</param>
    void ChangeState(std::unique_ptr<IGameState> newState);

    // 供状态类调用的公共接口
    /// <summary>
/// PlayingState から呼ばれ、Stage のスクロールなど背景更新を行う。
/// </summary>
    void UpdateStageInternal();
    /// <summary>
/// UI ボタンの入力を更新し、リスタートやタイトル遷移の要求を処理する。
/// </summary>
    void UpdateUIInternal();
    /// <summary>
/// GameLogicManager にゲーム中の入力判定・爆発・衝突判定を更新させる。
/// </summary>
    void UpdateGameLogicInternal();
    /// <summary>
/// 各 Ball と Goal の位置、アニメーション、移動 Strategy を更新する。
/// </summary>
    void UpdateBallsAndGoalsInternal();
    /// <summary>
/// すべての Goal が必要到達数を満たしているか確認する。
/// </summary>
    bool IsLevelComplete() const;
    /// <summary>
/// 外部のシーン管理に終了を通知するため、シーン終了フラグを立てる。
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
/// Ball の生成処理を差し替えるための Factory を設定する。
/// </summary>
/// <param name="factory">GameScene が所有する BallFactory。</param>
    void SetBallFactory(std::unique_ptr<IBallFactory> factory);

private:
    KamataEngine::DirectXCommon* dxCommon_ = nullptr;
    KamataEngine::Input* input_ = nullptr;

    std::unique_ptr<Stage> stage_;
    std::unique_ptr<GameUI> gameUI_;
    GameLogicManager gameLogicManager_;
    std::unique_ptr<IBallFactory> ballFactory_;

    std::vector<std::unique_ptr<MyEngine::Ball>> balls_;
    std::vector<std::unique_ptr<MyEngine::Goal>> goals_;

    KamataEngine::Camera camera_;

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

    KamataEngine::Vector2 startSize_ = {};
    KamataEngine::Vector2 targetSize_ = {};

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
/// チュートリアル画像と開始演出画像を読み込み、Sprite を生成する。
/// </summary>
    void LoadTutorialTextures();
    /// <summary>
/// クリック入力に応じてチュートリアル画像を次へ進める。
/// </summary>
    void UpdateTutorial();
    /// <summary>
/// チュートリアル終了後、開始演出へ移るまでの待機時間を更新する。
/// </summary>
    void UpdateStartWait();
    /// <summary>
/// 開始画像の拡大縮小アニメーションを進め、完了後にゲームを開始する。
/// </summary>
    void UpdateStartAnim();
    /// <summary>
/// 開始演出を終了し、ゲーム本編の PlayingState へ切り替える。
/// </summary>
    void StartGame();
    /// <summary>
/// 現在の進行状態に応じてチュートリアル画像または開始演出画像を描画する。
/// </summary>
    void DrawTutorial();

    /// <summary>
/// 現在のレベル設定から Ball と Goal を再生成し、ゲームロジックへ渡せる状態にする。
/// </summary>
    void InitializeLevelObjects();
    /// <summary>
/// Goal の達成状態を確認し、クリアしていれば GameOver 状態へ切り替える。
/// </summary>
    void CheckLevelCompletion();
    /// <summary>
/// 全 Goal の到達カウントをリセットし、リスタート時のクリア判定を初期化する。
/// </summary>
    void ResetLevelCompletion();
    /// <summary>
/// ゲームクリア時に GameOverState へ遷移し、外部シーン遷移の準備を行う。
/// </summary>
    void GameOver();
};

} // namespace MyEngine
